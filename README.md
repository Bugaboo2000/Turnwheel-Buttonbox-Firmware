
# Turnwheel Button Box Linux Firmware (Virtualization-Based Input Workaround)

## 🇺🇸 English

⚠️ **Experimental Project Notice**
This project is experimental and not fully stable. Behavior may vary depending on specific Turnwheel hardware revisions and system configurations. Certain edge cases and device-specific quirks may not yet be handled correctly.

---

## Overview

This project provides a low-level workaround for Turnwheel Button Box devices on Linux systems by leveraging input virtualization techniques to normalize and remap device behavior.

Certain Turnwheel button boxes may expose inconsistent or non-standard input reports under Linux, especially when interfacing with software relying on SDL2 (Simple DirectMedia Layer 2), such as racing simulators, games, and Wine-based applications. This can result in improper device recognition, missing button inputs, or incorrect rotary encoder behavior.

This "firmware" is not a traditional hardware-level firmware. Instead, it operates as a user-space abstraction layer that intercepts raw input events and re-emits them through a virtualized input device with standardized semantics.

---

## Technical Approach

The solution is based on the following pipeline:

### Raw Input Capture

* Interfaces directly with `/dev/input/event*`
* Captures low-level evdev events from the Turnwheel button box

### Input Normalization Layer

* Translates vendor-specific or malformed input data
* Fixes button states, encoder steps, and inconsistencies
* Converts rotary encoder signals into usable inputs

### Virtual Device Injection

* Uses the Linux `uinput` subsystem
* Creates a virtual controller with standardized behavior

### SDL2 Compatibility Layer

* Ensures correct detection by SDL2-based software

Works with:

* Native Linux games
* Wine / Proton environments
* Racing simulators (e.g., Assetto Corsa, Dirt Rally)

---

## Key Features

* Improved SDL2 compatibility
* Proper handling of rotary encoders (turnwheel inputs)
* Works with Wine and Proton
* Real-time input translation
* No kernel patches required
* Fully user-space implementation

---

## Limitations

* Experimental and not fully reliable
* Hardware-dependent behavior (Turnwheel models may differ)
* Requires background execution
* Needs proper permissions for `/dev/input/event*`
* May require custom udev rules
* Rotary encoder resolution may vary depending on device

---

## Conclusion

This project acts as a compatibility bridge for Turnwheel Button Box devices on Linux by virtualizing input behavior. While not perfect, it provides a practical workaround for improving device recognition and usability in environments where native support is limited.

---

## 🇧🇷 Português

⚠️ **Aviso: Projeto Experimental**
Este projeto é experimental e não está totalmente estável. O comportamento pode variar dependendo da revisão do hardware Turnwheel e da configuração do sistema. Algumas peculiaridades e casos específicos ainda podem não ser tratados corretamente.

---

## Visão Geral

Este projeto fornece um workaround de baixo nível para Button Box Turnwheel em sistemas Linux, utilizando técnicas de virtualização de entrada para normalizar e corrigir o comportamento do dispositivo.

Alguns button boxes Turnwheel podem apresentar relatórios de entrada inconsistentes ou fora do padrão no Linux, especialmente ao interagir com softwares que utilizam SDL2 (Simple DirectMedia Layer 2), como simuladores de corrida, jogos e aplicações via Wine. Isso pode resultar em falhas de reconhecimento, botões não detectados ou comportamento incorreto de encoders rotativos.

Este "firmware" não é um firmware tradicional de hardware. Em vez disso, funciona como uma camada de abstração em espaço de usuário que intercepta eventos de entrada brutos e os reemite através de um dispositivo virtual com comportamento padronizado.

---

## Abordagem Técnica

A solução segue o seguinte fluxo:

### Captura de Entrada Bruta

* Interface direta com `/dev/input/event*`
* Captura eventos evdev do button box Turnwheel

### Camada de Normalização

* Tradução de dados específicos do dispositivo
* Correção de botões, encoders e inconsistências
* Conversão de sinais de encoder rotativo em entradas utilizáveis

### Injeção de Dispositivo Virtual

* Utiliza o subsistema `uinput` do Linux
* Cria um controle virtual padronizado

### Compatibilidade com SDL2

* Garante reconhecimento correto por aplicações

Compatível com:

* Jogos nativos Linux
* Wine / Proton
* Simuladores de corrida (ex: Assetto Corsa, Dirt Rally)

---

## Principais Características

* Melhor compatibilidade com SDL2
* Suporte adequado para encoders rotativos (turnwheel)
* Funciona com Wine e Proton
* Tradução de entrada em tempo real
* Não requer modificações no kernel
* Implementação totalmente em espaço de usuário

---

## Limitações

* Projeto experimental e não totalmente confiável
* Comportamento dependente do hardware (variações entre modelos Turnwheel)
* Necessita execução em segundo plano
* Requer permissões para `/dev/input/event*`
* Pode exigir regras udev personalizadas
* Precisão do encoder pode variar conforme o dispositivo

---

## Conclusão

Este projeto atua como uma camada de compatibilidade para Button Box Turnwheel no Linux através da virtualização da entrada. Apesar de não ser uma solução perfeita, oferece um workaround prático para melhorar o reconhecimento e uso do dispositivo em ambientes onde o suporte nativo é limitado.

---

## Legal Disclaimer

This project is an independent, community-driven effort and is not affiliated with, endorsed by, or associated with Turnwheel in any way.

All trademarks, product names, and brand names are the property of their respective owners. This project does not contain or distribute any proprietary firmware or intellectual property.
