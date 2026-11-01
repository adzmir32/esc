# Problems met and how they were solved

| Symptom | Cause | Fix or status |
| --- | --- | --- |
| Configurator "Did not connect" through the FreelyRC USB linker; a raw handshake from PowerShell returned 0 bytes, not even an echo | Single-wire configurators expect their own bytes echoed back. The linker most likely has no receive line tied to the signal pin (not measured) | Not fixed. Used the FC passthrough instead |
| "BAD OR NO ACK FROM ESC" through the first FC | That FC's IMU is broken, so ArduCopter loops in a configuration error ("INS: unable to initialise driver") and never installs the passthrough handler | Used an FC with a working IMU |
| Windows Multi ESC Config Tool says "No settings found" on all four ESCs while am32.ca reads the same ESCs normally | Unknown; the tool's source is not available | Used am32.ca |
| am32.ca shows ESC 3 (U7) as "did not respond" on board 1; an ESP32 handshake on J1 pin 7 gets no reply | Abnormal diode readings on that signal net (1.06 V and 0.48 V against OL and 0.8 V on the other pins); SWD readback and the series resistor R57 (221 ohm against R87's 223 ohm) are normal | Open. Board 2 is used instead |
| `SERVO4_FUNCTION` not in the Mission Planner list | Incomplete parameter download | Refresh Params, then it appeared |
| Motor Test circle did not match the frame | Motors sit in different corners than ArduCopter's Quad X numbering assumes | Remapped `SERVO1` to `SERVO4_FUNCTION` (see motor-mapping.md) |
| Reversed ticks saved in am32.ca but spin direction did not change | The ESCs had not reloaded their settings, and the page's card numbers do not follow output order | Cold restart (ESC and FC supplies off for 30 seconds), then used a card table built from test results |
| "USB device not recognized" after unplugging the FC USB | USB was plugged in before the FC finished booting | Power the FC first, wait for the boot, then plug in the USB |
| An early board smoked at power-up | Unknown short (about 10 ohm across the supply) | Board set aside |

## Signal path of one channel (from the rev 3.4 netlist)

| Channel | MCU | J1 pin | Series resistor | Motor pads (U3) |
| --- | --- | --- | --- | --- |
| M1 | U2 | 5 | R20 (220 ohm) | 21 to 23 |
| M2 | U5 | 6 | R39 (220 ohm) | 18 to 20 |
| M3 | U7 | 7 | R57 (220 ohm) | 15 to 17 |
| M4 | U9 | 8 | R87 (220 ohm) | 12 to 14 |

J1 pin to resistor pad 1, resistor pad 2 to the MCU's PB4 (pin 25). Nothing else is on the PB4 net.
