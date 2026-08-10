# Legacy AI Models

The generated sources below are intentionally absent from the active working tree. They remain recoverable from Git for competition review and retrospective testing.

| Model | Last archived source commit | Replaced by | Reason removed from active tree |
| --- | --- | --- | --- |
| `facedet` | `a8eb541` | YuNet | Float model was too large and too slow for the STM32H743 workflow. |
| `faceid` | `a8eb541` | SFace/OpenMV production path | Float model was too large and too slow for the required decision window. |
| `water_detect` | `5e1c295` | `waterlevel` | Earlier water model is not part of the validated ROI and preprocessing pipeline. |

## Restore For Retrospective Testing

```powershell
git restore --source=a8eb541 -- X-CUBE-AI/App/facedet* X-CUBE-AI/App/faceid*
git restore --source=5e1c295 -- X-CUBE-AI/App/water_detect*
```

After restoring, do not add these models to the production build alongside YuNet, SFace, and `waterlevel`. Use a separate diagnostic branch or build configuration.
