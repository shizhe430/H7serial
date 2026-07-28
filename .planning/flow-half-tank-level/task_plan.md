# Flow Calibration, Half-Cup Fallback, Tank Display, and ESP32 Reporting

## Goal
Keep the measured flow calibration and visual-first half-cup fallback, make PB5 display-only on OLED, and diagnose/fix STM32-to-ESP32 completed-drink reporting.

## Success Criteria
- Full-cup pump runtime is calculated from the supplied log and used for volume reporting.
- Half-cup visual stopping remains primary; a calibrated timeout stops dispensing if visual detection misses.
- PB5 input polarity and behavior match the reference project.
- PB5 changes only the OLED tank-level display and never changes pump behavior, workflow, speech, or ESP32 reporting.
- CubeMX `.ioc` and generated GPIO source remain aligned.
- STM32 sends a valid `STM_FP`/`STM_DONE` sequence over USART3 when a session completes.
- Release build succeeds in formal pump mode.

## Phases
- [completed] 1. Analyze the new full-cup log and inspect the PB5 reference implementation.
- [completed] 2. Design the smallest safe state-machine changes and CubeMX-aligned GPIO configuration.
- [completed] 3. Implement flow calibration, half fallback, and tank-level protection.
- [completed] 4. Build and audit all stop/reporting paths.
- [completed] 5. Remove PB5 from pump control and map it only to the OLED water-level field.
- [completed] 6. Audit and correct STM32 ESP32 completion reporting.
- [completed] 7. Rebuild and provide focused hardware verification.

## Constraints
- Preserve the verified automatic, voice, manual, abnormal, fingerprint, OLED, and ESP32 paths.
- Keep visual half/full detection as the primary stopping mechanism.
- Treat the supplied full cup as 400 ml.
- PB5 must never block or stop any dispensing mode.

## Errors Encountered
- Initial planning catch-up used the default `.codex` path, but this skill is installed under `.agents`; reran with the actual path.
- A recursive reference-project text search included the binary ELF and produced unusable output; restricted subsequent inspection to `Core/Inc`, `Core/Src`, and `.ioc` text files.
- One multi-file patch omitted the planning-file update header and failed validation; no file was changed by that attempt, then the corrected patch was applied.
- The skill completion helper only inspected the legacy root plan and reported `0/0`; the active scoped plan was manually verified with all four phases completed.
