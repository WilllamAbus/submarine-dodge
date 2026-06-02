## AK Embedded Base Kit - STM32L151 - Lập trình nhúng vi điều khiển

Game submarine dodge

An action submarine game developed on **AK Embedded Base Kit STM32L151** using the AK kernel event-driven architecture.

## Table of Contents
- [About](#about)
- [Features](#features)
- [Gameplay](#gameplay)
- [Boss Battle](#boss-battle)
- [Game Screens](#game-screens)
- [Hardware](#hardware)
- [Project Structure](#project-structure)
- [Build & Flash](#build--flash)
- [Development Environment](#development-environment)
- [Author](#author)

## About

Submarine Dodge is an action game where the player controls a submarine to dodge and destroy enemy submarines. The game is built on top of the **AK kernel** — an event-driven, task-based embedded OS — running on an **STM32L151** microcontroller with a 128x64 OLED display.

The game features a progressive difficulty system, a Boss battle, a Ranking board stored in EEPROM, and configurable Settings.

## Features

| Feature | Description |
|---|---|
|  Game Menu | Navigate between game, ranking, and settings |
|  Water Environment | Animated water background with bubbles |
|  Enemy Submarines | Auto-spawning enemies that shoot torpedoes |
|  Boss Battle | Boss appears at 200 points with 100 HP |
|  Ranking | Top 3 scores saved to EEPROM |
|  Settings | Sound ON/OFF, Speed Easy/Normal/Hard, Reset scores |
|  Loading Screen | Animated progress bar before game starts |
|  Victory Screen | Displayed when Boss is defeated |
|  Game Over Screen | Displayed when is defeated |

## Boss Battle

- Boss appears when score reaches **200 points**
- Boss HP: **100** (each torpedo deals 10 damage)
- Boss fires **2 parallel torpedoes** automatically
- Boss moves up and down automatically
- All enemy submarines disappear when Boss appears
- Defeat Boss → **Victory Screen**

---

## Build & Flash

### Requirements
- Ubuntu / WSL2
- GNU Arm Embedded Toolchain 10.3-2021.10
- AK Flash tool

### Build
```bash
cd application
make
```

### Flash
```bash
ak_flash /dev/ttyUSB0 build_submarine-dodge-application/submarine-dodge-application.bin 0x08003000
```
## Development Environment

| Tool | Version |
|---|---|
| OS | Ubuntu 22.04 / WSL2 |
| Compiler | arm-none-eabi-gcc 10.3.1 |
| IDE | VS Code |
| Flash Tool | AK Flash |

## Architecture

The game uses the **AK kernel** event-driven architecture:
- Each game object runs as an independent **Task**
- Communication via **Signals** (messages)
- **Timer** triggers periodic game loop (150ms tick)
- **State machine** manages game states (PLAYING / GAME_OVER)

## Author

Developed by **[Thai Kha](https://github.com/WilllamAbus)**
Based on **[AK Embedded Base Kit](https://github.com/the-ak-foundation/ak-base-kit-stm32l151)** by AK Foundation.
Demo video: [STM32L151](https://youtu.be/3-FSJyn7Pys)
---

*Inspired by Archery Game — AK Embedded Software*



[<img src="hardware/images/ak-foundation-logo.png" width="240"/>](https://github.com/the-ak-foundation)

Heartfelt thanks to [EPCB](https://epcb.vn/pages/frontpage) for standing with the AK Foundation and empowering our mission.

AK Embedded Base Kit is an evaluation kit for advanced embedded software learners.
- The KIT integrates 1.54" Oled LCD, 3 push buttons, and 1 Buzzers that play music, to learn the event-driven system through hands-on game machine design.
- The KIT also integrates RS485, Qwiic Connect System, and Grove Ecosystems, suitable for prototyping practical applications in embedded systems.

[<img src="hardware/images/ak-embedded-base-kit-version-3.jpg" width="480"/>](https://epcb.vn/products/ak-embedded-base-kit-lap-trinh-nhung-vi-dieu-khien-mcu)

### Memory map
- [ 0x08000000 ] : **Boot** [[ak-base-kit-stm32l151-boot.bin]](https://github.com/ak-embedded-software/ak-base-kit-stm32l151/blob/main/hardware/bin/ak-base-kit-stm32l151-boot.bin)
- [ 0x08002000 ] : **BSF** [ Memory for data sharing between Boot and Application ]
- [ 0x08003000 ] : **Application** [[ak-base-kit-stm32l151-application.bin]](https://github.com/ak-embedded-software/ak-base-kit-stm32l151/blob/main/hardware/bin/ak-base-kit-stm32l151-application.bin)

**Note:** After loading boot & application firmware, you can use [AK - Flash](https://github.com/ak-embedded-software/ak-flash) to load the application directly through the **USB** port on the KIT
```sh
ak_flash /dev/ttyUSB0 ak-base-kit-stm32l151-application.bin 0x08003000
```

### Hardware
**Schematic** [[schematic-ak-embedded-base-kit-version-3.pdf]](https://github.com/ak-embedded-software/ak-base-kit-stm32l151/blob/main/hardware/schematic/schematic-ak-embedded-base-kit-version-3.pdf)

[<img src="hardware/images/board-view-top.png" width="480"/>](https://epcb.vn/products/ak-embedded-base-kit-lap-trinh-nhung-vi-dieu-khien-mcu)

[<img src="hardware/images/board-view-bottom.png" width="480"/>](https://epcb.vn/products/ak-embedded-base-kit-lap-trinh-nhung-vi-dieu-khien-mcu)

### Reference
| Topic | Link |
| ------ | ------ |
| Blog & Tutorial | https://epcb.vn/blogs/ak-embedded-software |
| Where to buy KIT? | https://epcb.vn/products/ak-embedded-base-kit-lap-trinh-nhung-vi-dieu-khien-mcu |
