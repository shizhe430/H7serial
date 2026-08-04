# OpenMV Face-Speed Comparison

## Goal

Validate an OpenMV-style grayscale Haar detector plus LBP descriptor matcher in an isolated camera-1 diagnostic path before deciding whether to replace the active YuNet/SFace backend. Do not change the LCD, touch, pump, camera-0 water-level, dispensing state machine, or existing YuNet/SFace enrollment.

## Phases

- [complete] Measure the current capture/decode/preprocess/YuNet/SFace pipeline and compare it with OpenMV's H743 face workload class.
- [complete] Identify the highest-return optimization that preserves the product-flow contract.
- [complete] Acquire and audit an OpenMV-compatible Haar/LBP test implementation, cascade data, license, and required image contract.
- [complete] Build a separate camera-1 JPEG-only diagnostic firmware and PC Haar/LBP enrollment/matching harness.
- [complete] Build and run isolated camera-1 visual/enrollment validation.
- [in_progress] Port the same workload to a board-only diagnostic mode and measure H743 latency.
- [pending] Run face accuracy and full product-flow regressions before considering a production backend change.

## Success Criteria

- Explain the OpenMV speed difference using comparable workloads, not board name alone.
- Produce one ordered optimization flow with expected benefit, risk, and stop criteria.
- Preserve all normal product behavior outside the camera-1 face window.
- Any source change must pass the Release build and CubeMX regression check.

## Constraints

- Preserve YuNet/SFace as a compile-time rollback implementation.
- Do not alter camera-0 water-level inference, LCD touch behavior, or pump safety logic.
- Keep the milestone commit `a8eb541` as the rollback point.

## Errors Encountered

| Error | Attempt | Resolution |
|---|---:|---|
| GitHub push unavailable | prior session | Local milestone commit exists; retry push when connectivity returns |
| OpenMV GitHub source clone unavailable | 1 | Used local model reports and known OpenMV workload classes; do not claim same-model performance |
| Gitee/GitCode OpenMV mirrors unavailable | 1 | Gitee requested credentials and GitCode requested an interactive login; try raw/CDN sources and local OpenCV cascade fallback |
