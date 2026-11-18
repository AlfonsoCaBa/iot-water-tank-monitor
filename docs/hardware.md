# Components and container

## Initial inventory

| Component | Quantity | Status / purpose |
| --- | --- | --- |
| ESP32 board | 1 | Available; model to be confirmed. Processing and Wi-Fi. |
| HC-SR04 | 1 | Available; confirm the VCC, TRIG, ECHO, and GND labels. |
| Windows computer | 1 | Available. Programming and viewing the dashboard. |
| Container | 1 | Available; shape and dimensions still need to be evaluated. |

## Materials to gather or buy if missing from the kit

| Material | Suggested quantity | Purpose |
| --- | --- | --- |
| 830-point breadboard | 1 | Make connections without soldering. Check how much space the board occupies. |
| Male-to-male and male-to-female jumper wires | One set of each | Connect the breadboard, resistors, and modules; confirm the board's connectors. |
| **2.2 kΩ** resistor | 1 | Upper resistor in the ECHO voltage divider. |
| **3.3 kΩ** resistor | 1 | Lower resistor in the ECHO voltage divider. |
| USB data cable compatible with the board | 1 | Power and program the ESP32. |
| Ruler and measuring jug | 1 of each | Measure dimensions and known quantities of water. |
| Rigid sensor mount | 1 | Keep the sensor level and fixed above the container. |

Use 1/4 W resistors, preferably with 1% tolerance. Borrowing a multimeter from the laboratory is recommended for checking continuity and voltages. Confirm whether the board already has soldered pin headers before buying connectors.

## Recommended container

Suggested purchase: a **3 L plastic beaker with a wide opening and a body as cylindrical as possible**. It provides a wide opening for mounting the sensor and allows the water to be seen during tests.

One specific reference is the **VITLAB Griffin PP 3000 ml beaker, catalog number 616941**, listed by the manufacturer with a diameter of 170 mm and a height of 212 mm. This is a size reference; the brand is not required, and availability and price in Mexico have not been checked. An inexpensive or borrowed equivalent may work. [Manufacturer's specifications](https://www.vitlab.com/products/general-lab-products/beakers/griffin-beakers-pp-molded-graduations).

Before choosing a container, check that its base is stable and its inner walls do not taper significantly. A soda bottle with a neck and shoulders makes mounting and the height-to-volume relationship more difficult.

**Nominal capacity and external dimensions are not enough to calculate the amount of water.** We will measure the inside and add known volumes. If the cross-sectional area changes with height, we will build a calibration table. Markings on an ordinary container are a guide; the accuracy of our reference measurement will also be documented.

We propose starting with approximately **5 cm between the transducers and the water at the maximum fill level**; this mounting clearance must be tested. It may require raising the mount above the rim. The nominal 3 L will not automatically be treated as the usable capacity of our setup.

## HC-SR04 operating considerations

The standard module's datasheet specifies a 5 V supply and a minimum distance of 2 cm. These values guide the design; they do not demonstrate accuracy with our container. [HC-SR04 datasheet](https://cdn.sparkfun.com/datasheets/Sensors/Proximity/HCSR04.pdf).

The kit's HC-SR04 must be kept dry, without submerging it or exposing it to splashes. We will use it for a supervised tabletop prototype. Stability in the presence of nearby walls, moving water, and condensation must be evaluated physically.

## Details to confirm

- Exact ESP32 model, USB connector type, and availability of a suitable sensor power pin.
- Materials already included in the kit and access to a laboratory multimeter.
- Shape, internal dimensions, and usable capacity of the selected container.
