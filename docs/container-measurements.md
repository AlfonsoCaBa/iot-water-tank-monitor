# Stage 2 — Container observations

**Date:** November 20, 2026. **Evidence:** measurements and seven photographs supplied by the student. These are not independently measured dimensions or a completed volume calibration. The firmware remains unchanged.

## Selected container and reported dimensions

The photographs show a transparent plastic container with molded volume graduations, a threaded opening, and a shaped lower base/foot. The student describes it as cylindrical with a small lower narrowing. The exact wetted internal geometry is not established by the photographs.

| Reported quantity | Value | Evidence / interpretation |
| --- | --- | --- |
| Bottom-to-rim height | 23 cm | [Height photograph](images/container/container-total-height.png); exact internal/external baseline not confirmed. |
| Rim to start of lower feature | 22 cm | [Body photograph](images/container/container-body-height.png). |
| Lower feature height | 1 cm | [Base detail](images/container/container-lower-feature.png); may include an external foot rather than a water-filled taper. |
| Top diameter | 12.6 cm | [Top photograph](images/container/container-top-diameter.png); internal water diameter versus rim/external diameter not confirmed. |
| Lower feature diameter | 9.2 cm | [Lower photograph](images/container/container-lower-diameter.png); cannot automatically be used as the internal water diameter. |
| Highest reported graduation | 2500 ml | Molded nominal marking, not independently verified capacity. |
| Bottom to 2500 ml mark | 19.6 cm | [Graduation photograph](images/container/container-2500ml-mark.png); baseline subject to the same internal/external ambiguity. |
| 2500 ml mark to rim | 3.4 cm | Same photograph; useful for planning sensor clearance. |
| Preliminary sensor-to-bottom reading | 21.9 cm | Single user-reported reading; mount position and a repeated series have not been documented. |

[Container overview](images/container/container-overview.png). Photographs are preserved as supplied, including the student's annotations; they are not to-scale engineering drawings.

## What the dimensions do and do not establish

`22 + 1 = 23 cm` and `19.6 + 3.4 = 23 cm` are consistent length totals. They do not establish the internal water height or exact capacity.

For comparison only, an ideal straight cylinder with an **internal** diameter of 12.6 cm and a water depth of 19.6 cm would contain approximately **2444 ml**, not exactly 2500 ml. This is a hypothetical geometry calculation, not a capacity measurement or proof that the molded marking is wrong. Approximate measurements, rim thickness, bottom references, and marking tolerances may account for differences. A cylinder-plus-frustum model would require additional assumptions about the internal lower shape, so it will not be adopted from the pictures alone.

The 1.1 cm difference between 23 cm and 21.9 cm is not automatically a sensor error. Potential explanations include the transducer face sitting below the rim, an external base included in the ruler measurement, a different reflecting surface, or measurement error. No cause has been verified; do not add a 1.1 cm correction in software.

## Sensor clearance and usable maximum

The 2500 ml marking is a **candidate** upper operating level. It is not yet the calibrated usable capacity or a measured full-distance reference.

If the 21.9 cm empty reading and 19.6 cm marking height shared the same bottom reference and sensor position, their difference would be only **2.3 cm**. This is conditional arithmetic, not an observed water reading. It is close to the HC-SR04's specified 2 cm minimum distance. See the [datasheet](https://cdn.sparkfun.com/datasheets/Sensors/Proximity/HCSR04.pdf).

Use a fixed mount that leaves approximately **5 cm or more of actual clearance** between the transducer faces and the intended maximum water level, as a practical initial margin to test. With the reported 3.4 cm rim-to-mark distance, placing the transducer faces roughly 2 cm above the rim suggests about 5.4 cm clearance to that marking; verify it directly with a ruler rather than treating this estimate as calibration.

Changing the mount invalidates the preliminary 21.9 cm reference. Collect fresh readings after fixing the mount.

## Selected calibration approach

Build a table of **known added water volume versus measured sensor distance**. This avoids assuming the lower feature is a wetted cone or that height percentage equals volume percentage. Later, explain and implement piecewise linear interpolation between measured points; this will still be an approximation that needs validation.

Use a separate graduated jug or cylinder as the volume reference when available, and document its resolution/uncertainty. The container's own markings can be used for a preliminary demonstration, but comparing against the same markings does not independently establish volume accuracy.

Start with only two data sets: **empty** and **500 ml added**, collecting ten valid readings at each with the mount unchanged. Wait for the water to settle. Preserve errors or unstable readings as observations, rather than deleting them without a record. Additional volumes, including lower-volume points to characterize the base, and an independently measured upper endpoint will follow after these first readings are reviewed.

| Data set | Actual known volume | Raw sensor readings | Status |
| --- | --- | --- | --- |
| Fixed-mount empty | 0 ml after emptying | Not supplied. | Pending. |
| First water point | 500 ml target, to be measured independently | Not supplied. | Pending. |
| Remaining intermediate points | To be collected | Not supplied. | Pending. |
| Full reference / usable capacity | To be established with sufficient clearance | Not supplied. | Pending. |

Do not calculate a final liters factor, full distance, or calibrated percentage from the preliminary single empty reading. Water-surface response and repeatability remain untested.
