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
- [deferred] Hardware mux purchase and production wiring. Current stage uses direct-parallel DVP plus separate PWDN control.
- [complete] Build a temporary PWDN-only direct-parallel prototype before adding the production mux.
- [complete] Validate both cameras electrically: both read OV2640 ID and both produce valid JPEG frames.
- [complete] Validate the second camera image path: fixed face camera view shows a clear live image.
- [complete] Add shared camera switching/recovery code for water camera and face camera.
- [in_progress] Add a pre-dispense face identity stub: switch cam0 -> cam1 -> latch id or 0 -> switch cam1 -> cam0.
- [in_progress] Add a standalone face-camera diagnostic mode that does not touch pump or water-level inference.
- [pending] Collect face-camera samples from the mounted position.
- [pending] Train/analyze a constrained INT8 face model that fits internal Flash/RAM.
- [pending] Replace the stub with real face inference and template matching/classification.
- [pending] Integrate pre-dispense identity state with automatic, voice, and mechanical modes.
- [pending] Run dual-camera endurance and complete workflow regression tests.

## Constraints
- Preserve the verified dispensing workflow and ESP32 reporting behavior.
- Do not commit planning, reports, temporary files, or generated build artifacts.
- Do not assume the OV7670 article is electrically identical to OV2640.
- All face inference must execute on STM32H743; ESP32-C6 remains communications-only.
- Identity stays non-blocking: first successful ID is latched, otherwise id=0.
- The direct-parallel camera bus is an experimental stage only and must use break-before-make PWDN switching.
- Do not allocate separate long-lived activation arenas for water and face networks; use a shared arena sized to the larger model.
- Do not run face recognition during dispensing. Face identity happens only in the pre-dispense window and falls back to id=0 on failure.
- Default firmware must remain APP_MODE_PUMP_CTRL + water camera with CAMERA_FACE_IDENTITY_ENABLE=0 until the switch-back test is run on hardware.

## Errors
- planning-with-files session-catchup script was absent at the documented path; continued with direct Git inspection.
- GitHub HTTPS push failed three times due TCP timeout/reset. SSH ports are reachable but no GitHub SSH public key is configured.
