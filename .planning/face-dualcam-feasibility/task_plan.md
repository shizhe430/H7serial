# Face Recognition Dual-Camera Feasibility

## Goal
Preserve the completed drinking-machine firmware and replace AS608 with H743-executed face recognition using two OV2640 cameras time-multiplexed into the single DCMI.

## Success criteria
- Normal firmware uses APP_MODE_PUMP_CTRL with demo and force-test modes disabled.
- Release build completes before commit.
- Only firmware source/configuration changes are committed and pushed.
- Feasibility covers camera topology, compute/memory limits, model/data flow, integration risks, and staged implementation.

## Phases
- [in_progress] Audit, restore normal mode, build, commit, and push firmware snapshot (local commit complete; push blocked by network).
- [complete] Inspect current camera, pin, memory, and AI architecture.
- [complete] Verify the referenced dual-camera method and authoritative hardware constraints.
- [complete] Produce a recommended architecture and migration workflow.
- [complete] Select an H743-centered topology and assign available control pins.
- [pending] Hardware purchase, mux wiring, second-camera mounting, and electrical validation.
- [in_progress] Build a temporary PWDN-only direct-parallel prototype before adding the production mux.
- [pending] Add dual SCCB, camera mux, capture diagnostics, and switching recovery code.
- [pending] Collect face-camera data and train/analyze a constrained INT8 face model.
- [pending] Integrate pre-dispense identity state with automatic, voice, and mechanical modes.
- [pending] Run dual-camera endurance and complete workflow regression tests.

## Constraints
- Preserve the verified dispensing workflow and ESP32 reporting behavior.
- Do not commit planning, reports, temporary files, or generated build artifacts.
- Do not assume the OV7670 article is electrically identical to OV2640.
- All face inference must execute on STM32H743; ESP32-C6 remains communications-only.
- Identity stays non-blocking: first successful ID is latched, otherwise id=0.
- The direct-parallel camera bus is an experimental stage only and must use break-before-make PWDN switching.

## Errors
- planning-with-files session-catchup script was absent at the documented path; continued with direct Git inspection.
- GitHub HTTPS push failed three times due TCP timeout/reset. SSH ports are reachable but no GitHub SSH public key is configured.
