# Stage 2 — Container calibration and volume

**Status: collecting fixed-mount water references.** Container photographs, approximate dimensions, molded markings, and a preliminary 21.9 cm sensor-to-bottom reading have been supplied; see the [measurement record](container-measurements.md). These do not yet establish usable capacity or final empty/full references. The firmware still reports distance only.

## 1. Identify the actual container

Record a photograph and the container's shape and internal dimensions. For a cylinder, measure the internal diameter and note whether it changes from bottom to top. For a rectangular container, measure internal length and width. Also record internal height and any curved bottom, shoulders, or other changes of section.

The selected container has been photographed and measured. Because the lower feature and measurement baselines are ambiguous, proceed with a known-volume calibration table rather than requiring more geometric measurements before starting the water test. Do not assume a 1 cm water-filled conical section from the external-looking base.

Do not use external dimensions or nominal capacity as an exact volume reference. A tapered or irregular container needs a geometry model or calibration table; height percentage cannot automatically be treated as volume percentage.

## 2. Fix the sensor position

Mount the sensor above the opening, horizontal and aimed vertically at the water surface, with both transducers clear of the rim and nearby obstacles. Keep the sensor dry. Disconnect USB before adjusting the wiring or mount.

The sensor must remain in the same position during empty/full calibration and later measurements. Remounting it invalidates those references. Start with approximately 5 cm of clearance above the intended maximum water level; this is a proposed mounting margin to test, not a measured performance guarantee.

The 2500 ml molded mark is reported to be only 3.4 cm below the rim. Plan to raise the transducer faces above the rim (roughly 2 cm as an initial setup), and directly verify approximately 5 cm or more of clearance to the intended maximum level. Recollect the empty series after mounting; do not reuse 21.9 cm automatically.

## 3. Collect the reference distances

- **Empty reference:** with the container empty, record several distance readings to its bottom. Preserve the raw series and compare with a ruler.
- **Full reference:** choose and mark a usable maximum water level that preserves sensor clearance. Fill using known quantities, record the total added volume, wait for the surface to settle, and collect another reading series.
- The empty distance must be greater than the full distance. Both references must lie within the sensor's valid measurement region and reflect the actual bottom/water surface rather than the rim or walls.
- Reject missing echoes and physically impossible readings before selecting representative references. Filtering will be explained and implemented after observing the data.

For the next physical step, collect ten empty readings, then add **500 ml measured with a separate graduated reference** and collect ten readings after the surface settles. Disconnect USB while adding water without disturbing the fixed mount or wiring, then reconnect to obtain the readings. If only the container's own markings are available, label those volumes as approximate and do not claim independent volume validation. Review these two sets before collecting the rest of the calibration curve. Keep the same distance-only sketch for now.

| Calibration input | Value |
| --- | --- |
| Container shape and photograph | Supplied; transparent cylindrical-looking container with a shaped lower base. |
| Internal dimensions and changes of section | Approximate reported measurements recorded; exact wetted geometry not established. |
| Sensor-to-bottom distance with empty container | Preliminary single reading: 21.9 cm; final fixed-mount series pending. |
| Sensor-to-water distance at marked maximum level | Pending; no full-water reading supplied. |
| Known volume at that maximum level | Pending; 2500 ml is a molded marking only. |
| Intermediate known-volume readings | Pending. |

## 4. Understand the level calculation before implementation

```text
level_percent_raw = 100 × (empty_distance - measured_distance)
                         / (empty_distance - full_distance)
level_percent = clamp(level_percent_raw, 0, 100)
```

All distances must use the same unit. Validate the references and measurement first; a missing echo or impossible reflection must not become a plausible percentage through clamping. The allowed treatment of small endpoint deviations will be chosen from observed measurement variation.

The formula gives **height percentage** between the calibrated endpoints. A constant internal cross-sectional area allows proportional conversion to volume; a changing area does not.

## 5. Calculate and validate liters

For an ideal straight cylinder with a flat bottom, `volume_liters = π × internal_radius_cm² × water_height_cm / 1000`. For a rectangular vessel with constant section, use `internal_length_cm × internal_width_cm × water_height_cm / 1000`. The measured geometry must justify the chosen model.

For this container, use measured distance-to-volume pairs and explain piecewise linear interpolation once the data are available. It accommodates the actual base and walls without assuming proportional volume versus height across the whole range. Preserve additional known-volume points for validation rather than using every observation to fit the table and then calling that independent validation. Label height percentage and volume percentage distinctly if they differ.

Before Stage 3, implement validation, filtering, and the selected table/interpolation approach, then compare estimated liters against several known water quantities and document the errors. The calibration table has not been collected and no volume algorithm has been implemented yet.
