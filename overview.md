# Overview: the ESC, the OpenESC 30x30 board and this project

## What an ESC is and what it is used for

A brushless motor has three coils and no brushes, so it cannot be driven by plain DC. An electronic speed controller (ESC) takes battery power and a throttle command from the flight controller (FC), and switches the three motor wires in the right order, thousands of times per second, so the motor turns at the requested speed. On a multirotor drone the FC sends a new throttle value to every motor many times per second, and the thrust from each motor is what keeps the drone level and moves it.

Each motor needs its own power stage. A **4-in-1 ESC** puts four of them on one small board, which saves wiring and weight on a quadcopter.

## The OpenESC 30x30

The board used here is the OpenESC 30x30, an open hardware design by Incutec BV (see `references.md` for the source and licence). This work used revision 3.3 / 3.4.

| Item | Detail |
| --- | --- |
| Type | 4-in-1 brushless ESC, one board for four motors |
| Mounting pattern | 30.5 x 30.5 mm (the usual "30x30" flight-controller stack) |
| Controllers | Four independent AT32F421G8U7 microcontrollers, U2, U5, U7 and U9, one per motor |
| Firmware | AM32, target `OPENESC_30_F421` |
| Motor drivers | 24 N-channel MOSFETs (SP40N01GHNK), six per motor |
| Power | Battery pad (+BATT) feeds an LMR54406 buck converter to +10 V, then a TLV76733 regulator to +3V3 for the controllers |
| Control input | DShot (DShot300 was used) on one signal wire per motor |
| Connector | 8-pin JST SH connector (J1) to the flight controller |
| Board | Six layers |

The four controllers are fully separate. Each one has its own signal wire, its own bootloader, firmware and settings, and its own programming pads. If one channel has a fault, the other three are not affected.

The board has no reverse-polarity protection and no fuse. Use a fuse and check polarity on every connection.

## Pinout

### J1 (flight controller connector)

| Pin | Net | Use |
| --- | --- | --- |
| 1 | +BATT | Battery voltage. Do not connect it to anything else unless you mean to |
| 2 | GND | Ground, shared with the FC |
| 3 | CURR | Current-sense output |
| 4 | Not connected | No net on the board (netlist: unconnected pad) |
| 5 | M1 | Signal for motor 1, controller U2 |
| 6 | M2 | Signal for motor 2, controller U5 |
| 7 | M3 | Signal for motor 3, controller U7 |
| 8 | M4 | Signal for motor 4, controller U9 |

Each signal goes through a 220 ohm resistor (R20, R39, R57, R87) to pin PB4 of its controller. PB4 carries both the DShot throttle signal and the bootloader and configurator traffic.

### Motor pads

| Controller | Motor | Output pads |
| --- | --- | --- |
| U9 | M4 | 12, 13, 14 |
| U7 | M3 | 15, 16, 17 |
| U5 | M2 | 18, 19, 20 |
| U2 | M1 | 21, 22, 23 |

### Programming (SWD) pads

| Controller | Pads |
| --- | --- |
| U2 | TP1, TP4 |
| U5 | TP2, TP5 |
| U7 | TP3, TP6 (TP6 is SWCLK) |
| U9 | TP7 (SWDIO), TP8 (SWCLK) |

A 3.3 V check point is pad 1 of C66 (it read about 3.30 V on a powered board).

## What was done in this project

1. **Safe first power-up.** Checked for a short across the supply, fitted a capacitor, and powered through a 50 ohm resistor bank and a fuse from a bench supply, with the motor and battery disconnected. An early board that smoked at power-up (about 10 ohm across the supply) was set aside.
2. **Bootloader, firmware and EEPROM written over SWD.** Each blank controller received the AM32 bootloader (v19.0.0, PB4 build), the OPENESC_30_F421 firmware (2.21) and a 192-byte settings image, using OpenOCD and an ST-Link. Every write was verified and read back. Details: `bringup-and-flashing.md`.
3. **Checked the bootloader over the signal wire.** An ESP32 sketch (`sketches/bl_test`) sent the handshake and got the expected `34 37 31 14 1F 06 06 03 30` reply from a healthy channel.
4. **Found a fault on one channel of board 1.** The U7 (M3) signal net had abnormal diode readings, and its handshake got no reply, while its flash contents and resistor were fine. Board 1 stays open; a second, re-soldered board (board 2) was flashed and used. See `troubleshooting.md`.
5. **Read and configured the ESCs through the flight controller.** Passthrough from the FC with am32.ca read all four controllers on board 2 as Bootloader PB4, MCU F421, EEPROM v4, firmware OPENESC_30_F421 2.21. The first FC had a broken IMU and could not do this; a second unit worked.
6. **Bench-tested the motors** at low throttle through the resistor bank, with the ESP32 sketch `sketches/pwm_test` and then from Mission Planner. All four spun.
7. **Fitted the ESCs on the frame and set the motor mapping and spin directions.** The FC outputs were remapped in parameters and the reversed setting was set in am32.ca. The final Motor Test showed the correct order and directions. Details: `motor-mapping.md`.
8. **Documented the work** and put it in this repository.

## Current status and what is not done yet

| Area | Status |
| --- | --- |
| Board 2, all four controllers | Flashed, verified, read by am32.ca, motors spinning in the right order and direction |
| Board 1, U7 (M3) | Faulty signal net, not resolved |
| Full battery, propellers, hover | Not tested |
| 6S rating | Not confirmed. The 470 uF capacitor value and the board's voltage and current rating (capacitor 50 V, MOSFET class) need checking before 6S |
| Safety limits | Low-voltage cut-off, temperature and current limits are at their defaults |

For the full list, see the status and open issues sections of the main `README.md`.

## Where to go next in this repository

- `bringup-and-flashing.md` for the flashing steps and expected readbacks
- `motor-mapping.md` for FC parameters and the spin-direction table
- `troubleshooting.md` for problems met and how they were solved
- `references.md` for the source of every design, firmware and tool used
