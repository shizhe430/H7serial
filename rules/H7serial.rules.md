# H7serial Rules

This rules file tracks the active blueprint for the `STM32H743IIT6 + OV2640 + XCAM` bring-up work.

## Canonical Blueprint

The canonical project blueprint is:

- [H7serial.md](C:\Users\heshizhe\Desktop\STM32\H7serial\H7serial.md)

## Working Rule

Before changing camera, DCMI, DMA, SCCB, cache, or UART streaming logic:

1. Read `H7serial.md`.
2. Treat `H7serial.md` as the source of truth for completed steps, blocked issues, and test history.
3. Keep this rules file updated whenever the blueprint location or usage rule changes.
4. Keep `H7serial.md` updated whenever progress, findings, or blocking issues change.

## Current Goal

Make the STM32H743 project output a stable `324x224` OV2640 JPEG stream that can be displayed in XCAM.
