#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/uinput.h>
#include <linux/input.h>
#include <string.h>
#include <errno.h>
#include <sys/select.h>  // Para select() em leitura

int main() {
    int fd, ufd;
    struct input_event ev;
    struct uinput_setup usetup;
    fd_set readfds;

    // Abra o Arduino original (bloqueante para não perder eventos)
    fd = open("/dev/input/by-id/usb-Arduino_LLC_Arduino_Leonardo_HIDNB-if02-event-joystick", O_RDONLY);
    if (fd < 0) {
        perror("Erro abrindo Arduino");
        return 1;
    }

    // Crie uinput virtual
    ufd = open("/dev/uinput", O_WRONLY | O_NONBLOCK);
    if (ufd < 0) {
        perror("Erro abrindo uinput");
        return 1;
    }

    // Eventos suportados: Baseado no original (EV_KEY, EV_MSC, EV_SYN) + EV_ABS para forçar detecção como joystick
    ioctl(ufd, UI_SET_EVBIT, EV_KEY);
    ioctl(ufd, UI_SET_EVBIT, EV_MSC);  // Essencial para forward do MSC_SCAN
    ioctl(ufd, UI_SET_EVBIT, EV_ABS);  // Adicionado para ter "axes" no jstest/js*
    ioctl(ufd, UI_SET_EVBIT, EV_SYN);

    // Propriedades: Button box como gamepad/joystick
    ioctl(ufd, UI_SET_PROPBIT, INPUT_PROP_BUTTONPAD);
    ioctl(ufd, UI_SET_PROPBIT, INPUT_PROP_POINTING_STICK);

    // Bits de teclas: Exatamente os do evtest original (288-303, 704-719, incluindo 300)
    for (int i = 288; i <= 303; i++) {
        ioctl(ufd, UI_SET_KEYBIT, i);
    }
    for (int i = 704; i <= 719; i++) {
        ioctl(ufd, UI_SET_KEYBIT, i);
    }

    // Eixos mínimos: Adicione 2 eixos genéricos (ABS_X, ABS_Y) para que jstest mostre "2 axes" e Wine detecte como joystick
    // Eles ficam fixos em 0 (sem forward, pois original não tem ABS)
    ioctl(ufd, UI_SET_ABSBIT, ABS_X);
    ioctl(ufd, UI_SET_ABSBIT, ABS_Y);

    // Setup eixos com ranges padrão (fixos em 0)
    struct uinput_abs_setup abs_setup = {0};
    abs_setup.absinfo.fuzz = 0;
    abs_setup.absinfo.flat = 0;

    abs_setup.code = ABS_X;
    abs_setup.absinfo.minimum = -32768;
    abs_setup.absinfo.maximum = 32767;
    abs_setup.absinfo.value = 0;  // Fixo em centro
    ioctl(ufd, UI_ABS_SETUP, &abs_setup);

    abs_setup.code = ABS_Y;
    abs_setup.absinfo.minimum = -32768;
    abs_setup.absinfo.maximum = 32767;
    abs_setup.absinfo.value = 0;  // Fixo em centro
    ioctl(ufd, UI_ABS_SETUP, &abs_setup);

    // Nome e ID: "Turnwheel Button Box" com ID do Arduino original
    memset(&usetup, 0, sizeof(usetup));
    snprintf(usetup.name, UINPUT_MAX_NAME_SIZE, "Turnwheel Button Box");
    usetup.id.bustype = BUS_USB;
    usetup.id.vendor = 0x2341;   // Arduino original
    usetup.id.product = 0x8036;  // Arduino original
    usetup.id.version = 1;

    ioctl(ufd, UI_DEV_SETUP, &usetup);
    ioctl(ufd, UI_DEV_CREATE);

    sleep(1);

    // Calibração inicial: Envie eixos em 0 + SYN para ativar detecção de axes
    struct input_event syn = { .type = EV_SYN, .code = SYN_REPORT, .value = 0 };
    struct input_event abs_x = { .type = EV_ABS, .code = ABS_X, .value = 0 };
    struct input_event abs_y = { .type = EV_ABS, .code = ABS_Y, .value = 0 };
    write(ufd, &abs_x, sizeof(abs_x));
    write(ufd, &abs_y, sizeof(abs_y));
    write(ufd, &syn, sizeof(syn));

    printf("Turnwheel Button Box virtual criado com 2 axes fixos! Verifique jstest para '2 axes'.\n");

    // Loop com select() para leitura eficiente
    while (1) {
        FD_ZERO(&readfds);
        FD_SET(fd, &readfds);
        struct timeval tv = { .tv_sec = 1, .tv_usec = 0 };

        int ret = select(fd + 1, &readfds, NULL, NULL, &tv);
        if (ret < 0) {
            perror("Erro no select");
            break;
        } else if (ret == 0) {
            continue;
        }

        ssize_t res = read(fd, &ev, sizeof(ev));
        if (res == sizeof(ev)) {
            // Forward TODOS os eventos exatamente como no original (MSC, KEY value=0/1, SYN)
            // Não forward para ABS (original não tem, eixos ficam fixos)
            if (ev.type != EV_ABS) {  // Ignora se por acaso vier ABS (não deve)
                write(ufd, &ev, sizeof(ev));
            }
        } else {
            perror("Leitura incompleta");
        }
    }

    close(fd);
    close(ufd);
    return 0;
}