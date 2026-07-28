# ESP32-P4 Vibe Coding workspace

## Goal

Place the ESP32-P4 camera application in a self-contained project folder and align the local ESP-IDF/VS Code setup with the user-provided official EIM workflow so build, flash, and monitor are available directly from a Vibe Coding IDE.

## Success criteria

- The P4 application has no dependency on the STM32 project directory.
- The project opens as a VS Code/Cursor workspace with the ESP-IDF extension configured for ESP-IDF v5.5.5.
- One-command environment initialization and build scripts work from a normal PowerShell terminal.
- VS Code tasks exist for build, full clean, flash, monitor, and flash+monitor.
- Build succeeds from the final project location.
- Flash configuration is ready, with only the physical COM port left to select when hardware is connected.

## Phases

- [completed] Inspect the official workflow document and current ESP-IDF installation metadata.
- [completed] Choose and create the final standalone project/workspace layout.
- [completed] Add IDE, environment, build, flash, and monitor configuration.
- [completed] Validate clean build and inspect IDE-visible configuration.
- [completed] Document remaining hardware-only flash verification.

## Constraints

- Do not modify or move the STM32 project.
- Do not claim physical flashing succeeded without a detected serial device.
- Keep ESP-IDF components and pin mapping at the already verified versions/configuration.
