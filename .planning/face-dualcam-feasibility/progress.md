# Progress

- 2026-07-22: Audited Git status, branch, remotes, recent milestones, active macros, and source diff.
- 2026-07-22: Downloaded and inspected the referenced CSDN dual-camera article.
- 2026-07-22: Restored APP_MODE_PUMP_CTRL; timed-demo and force-pump-test remain disabled.
- 2026-07-22: Release build succeeded (text 316332, data 19232, bss 358556).
- 2026-07-22: Created local milestone commit 103db30 with five firmware source/config files.
- 2026-07-22: GitHub push failed after HTTPS/default, retry, and HTTP/1.1 attempts due network timeout/reset; branch is ahead by one commit.
- 2026-07-22: Verified H743 DCMI and memory constraints from ST datasheet/reference manual and OV2640 control/electrical behavior from its datasheet.
- 2026-07-22: Rejected external face coprocessor because the ST competition track requires H743 to remain the primary compute platform.
- 2026-07-22: Fixed a concrete H743 dual-camera mux topology, control-pin assignment, and implementation ownership split.
- 2026-07-22: Confirmed stedgeai.exe is installed locally and analyzed a public MobileFaceNet ONNX successfully; recorded the FP32 footprint and INT8 requirement.
- 2026-07-29: Hardware direct-parallel dual OV2640 test passed. Both PWDN-selected cameras read the expected ID and produce valid JPEG frames.
- 2026-07-29: Fixed second-camera view mode showed a clear image, so the next firmware step is shared switching/recovery infrastructure.
- 2026-07-29: Started implementing the common camera switching path before adding the face identity stub.
- 2026-07-29: Restored default firmware mode to APP_MODE_PUMP_CTRL with the water camera selected.
- 2026-07-29: Added common camera switching/recovery code and a macro-gated pre-dispense face identity placeholder.
- 2026-07-29: Release build passed with CAMERA_FACE_IDENTITY_ENABLE=0. Size: text 317692, data 19232, bss 358564.
