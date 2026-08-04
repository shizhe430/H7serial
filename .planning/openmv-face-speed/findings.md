# Findings

## Baseline

- MCU is STM32H743 at 480 MHz with 32 MB SDRAM running at 100 MHz.
- Current camera-1 chain is JPEG 320x240 decode to RGB888, 320x320 INT8 YuNet, five-landmark alignment, then 112x112 float32 SFace.
- Measured hardware baseline is about 0.85 s for YuNet and 4.43 s for SFace, about 5.33 s total when a face is detected.
- YuNet is INT8 end to end. SFace has quantized weights but float32 input/output and remains the dominant cost at about 615 M MAC.
- Current implementation performs global `SCB_CleanInvalidateDCache()` around model runs, which is broader than required and is a candidate for targeted optimization.
- Release builds generated AI code with Cortex-M7 hard-float, `-O3`, and X-CUBE-AI `-O time`; CPU/compiler settings are not the main remaining bottleneck.
- Current SFace accounts for about 83% of the measured full-face latency; YuNet accounts for about 16%.
- Product mode already invokes face identity once on stable-cup entry and skips SFace when YuNet reports no face. OpenMV-style low-cost gating is therefore already present.
- Current SFace is hybrid: its weights and convolution scratch paths are quantized, but its external input/output and many intermediate batchnorm/activation tensors remain float32. It is not an end-to-end INT8 MCU model.
- The local InsightFace `w600k_mbf.onnx` is also unsuitable as-is: float32 input/output, 447.5 M MAC, and about 13 MB weights.
- The local 128x128 BlazeFace detector is only 32.3 M MAC, but replacing YuNet first would also replace the five-landmark contract needed for alignment. That is lower priority and higher integration risk than replacing the recognizer.

## OpenMV Comparison

- OpenMV H7 examples commonly use grayscale Haar/feature detection, keypoint descriptors, or small closed-set INT8 classifiers. These are not computationally equivalent to a 320x320 YuNet plus a 614.8 M-MAC SFace embedding network.
- OpenMV avoids work through sensor-side grayscale/low-resolution capture, ROI processing, and event-driven inference. The transferable lesson is workload reduction, not a hidden H743 clock advantage.
- Running the same current SFace graph on OpenMV firmware would still be dominated by its MAC count and external-memory traffic.

## Selected Optimization Route

- Keep YuNet, JPEG/camera handling, five-landmark alignment, enrollment database, and pump state-machine API unchanged.
- Replace only the SFace backend behind `FaceAI_RunJpeg` with a 112x112 or 96x96, end-to-end INT8, 128-D embedding model targeting at most 150 M MAC, at most 2 MB activations, and at most 4 MB weights.
- Target hardware latency is at most 1.5 s for recognition and at most 2.5 s total. If the candidate misses either latency or accuracy limits, retain SFace as the default backend.
- Re-enrollment is expected because embedding values change even when the output remains 128-D.
- Cache-call cleanup is worth A/B measurement but cannot explain or remove a 4.4 s compute-bound SFace run.

## User-Selected OpenMV Route

- The target combination is OpenMV's classic grayscale Haar cascade detector plus LBP descriptor matching, not an ONNX pair.
- First integration should keep JPEG capture and the existing FAV1 viewer protocol, then decode to grayscale internally. Sensor-side grayscale/raw capture is a later optimization because changing DCMI format at the same time would widen the regression surface.
- The current `FaceAI_*` API can remain stable, but the enrollment record version and descriptor representation must change. Existing SFace enrollment must not be interpreted as an LBP record.
- GitHub git/raw access remained unavailable, but the official OpenMV master archive was acquired through `codeload.github.com` and audited locally.
- Official OpenMV uses a 25-stage 24x24 frontal-face Haar cascade and a 7x7-region, 59-bin LBP descriptor (2891 bytes) with face-region weights. Haar/LBP code is MIT; the frontal-face cascade carries the Intel OpenCV redistribution license.
- The first host harness used an 8x8 normalized LBP-family descriptor. Its accuracy result validates the workload family and scene, not byte-for-byte equivalence with OpenMV's 7x7 weighted descriptor.

## Guardrails

- Camera 0 water-level and pump-control behavior must not change.
- Face work remains event-driven on camera 1 and should run only inside the face authentication window.
- The stable rollback commit is `a8eb541`.

## Isolated Host Validation

- The test used the real camera-1 FAV1 JPEG stream; board inference flags remained `0x00`, so YuNet/SFace did not run.
- ID=1 enrollment averaged five consecutive LBP descriptors and was stored only in `tmp/openmv_face_id1.npz`.
- Same-person set: 42 valid face frames, distance `0.0603..0.4245` (median `0.2533`).
- Different-person set: 162 valid face frames, distance `0.6939..0.7222` (median `0.7092`).
- A threshold of `0.55` accepted 42/42 same-person frames and 0/162 different-person frames in this test. The observed separation margin was `0.2694`.
- PC mean processing time was 5.31 ms for Haar and 0.97 ms for LBP. This validates workload and test-scene accuracy only; it is not an H743 latency measurement.
- The official OpenMV LBP example labels the implementation as a proof of concept and warns that it is inadequate in general real-life conditions. The project's own scene-specific positive/negative test remains mandatory.
- A captured board frame with a backlit, glasses-wearing, slightly turned face produced no detections in either PC OpenCV Haar or the OpenMV XML at `scale=1.25`; the same raw frame detected at `scale=1.20`. The board diagnostic therefore uses `1.20` as a scene-driven recall adjustment while retaining the official cascade and threshold.
- Exact offline replay of the OpenMV quantized cascade showed the captured face passed only 5/25 stages at the official `threshold=0.5`. A diagnostic `10 stages / threshold=1.0 / scale=1.20` setting yielded a compact candidate cluster around the face; it must pass a no-face false-positive test before it can be considered usable.
- The relaxed 10-stage hardware mode produced unacceptable box jitter and false-trigger risk, so it was rejected. The strict diagnostic was restored to all 25 stages and aligned with the successful PC detector's equalized grayscale, `scale=1.1`, `minSize=30`, and five-neighbor grouping policy; false-positive control takes priority over angled-face recall.
- Board-side LBP initially reused the globally equalized Haar image and equalized the face crop a second time. It now preserves raw grayscale for the descriptor and uses the equalized copy only for Haar detection.
- Physical board calibration after that correction measured same-person chi-square distance `0.050..0.080` and different-person distance `0.130..0.241`. The PC threshold `0.55` was therefore invalid on the board path; the isolated board threshold is now a conservative `0.10`.
