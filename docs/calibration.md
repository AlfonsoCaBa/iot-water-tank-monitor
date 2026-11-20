# Stage 2 — Container calibration and volume

**Status: planning and data collection.** No container dimensions, capacity measurements, or empty/full distance references have been supplied. The firmware still reports distance only.

## 1. Identify the actual container

Record a photograph and the container's shape and internal dimensions. For a cylinder, measure the internal diameter and note whether it changes from bottom to top. For a rectangular container, measure internal length and width. Also record internal height and any curved bottom, shoulders, or other changes of section.

Do not use external dimensions or nominal capacity as an exact volume reference. A tapered or irregular container needs a geometry model or calibration table; height percentage cannot automatically be treated as volume percentage.

## 2. Fix the sensor position

Mount the sensor above the opening, horizontal and aimed vertically at the water surface, with both transducers clear of the rim and nearby obstacles. Keep the sensor dry. Disconnect USB before adjusting the wiring or mount.

The sensor must remain in the same position during empty/full calibration and later measurements. Remounting it invalidates those references. Start with approximately 5 cm of clearance above the intended maximum water level; this is a proposed mounting margin to test, not a measured performance guarantee.

## 3. Collect the reference distances

- **Empty reference:** with the container empty, record several distance readings to its bottom. Preserve the raw series and compare with a ruler.
- **Full reference:** choose and mark a usable maximum water level that preserves sensor clearance. Fill using known quantities, record the total added volume, wait for the surface to settle, and collect another reading series.
- The empty distance must be greater than the full distance. Both references must lie within the sensor's valid measurement region and reflect the actual bottom/water surface rather than the rim or walls.
- Reject missing echoes and physically impossible readings before selecting representative references. Filtering will be explained and implemented after observing the data.

| Calibration input | Value |
| --- | --- |
| Container shape and photograph | Pending. |
| Internal dimensions and changes of section | Pending. |
| Sensor-to-bottom distance with empty container | Pending. |
| Sensor-to-water distance at marked maximum level | Pending. |
| Known volume at that maximum level | Pending. |
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

If section varies or bottom geometry matters, use the appropriate volume model or measured height-to-volume pairs. Preserve intermediate known-volume points for validation rather than claiming accuracy from the two endpoints alone. Label height percentage and volume percentage distinctly if they differ.

Before Stage 3, implement validation and filtering, then compare estimated liters against several known water quantities and document the errors. No volume algorithm has been selected or implemented yet.
