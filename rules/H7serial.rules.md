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

Make the STM32H743 project output a stable `320x240` OV2640 JPEG stream that can be displayed in XCAM.

## Current Diagnostic Rule

- For XCAM black-screen cases with nonzero RX count, check JPEG frame extraction before changing DCMI polarity.
- Prefer the last complete `FFD8 ... FFD9` span in the DMA buffer; do not use the first forward match when residual bytes may exist in the buffer.
- The verified active H7 camera data mapping is:
  - `D5 = PD3`
  - `D6 = PB8`
  - `D7 = PB9`
