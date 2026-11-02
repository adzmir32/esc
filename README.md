# OpenESC-30x30 bring-up (AM32 on AT32F421)

Bring-up notes, procedures and test sketches for a custom 4-in-1 ESC (OpenESC-30x30 rev 3.3/3.4, four AT32F421G8U7 microcontrollers, AM32 firmware) built for a ~2 kg quadcopter. Work done during an OJT at Gamuda Tech, July to October 2026.

The ESC design derives from the open-source Open-4in1-AM32-ESC project. This repository holds the bring-up work only: how the chips were flashed, how the board was tested, how the motors were mapped, and what is still open.

## Status (20 October 2026)

| Check | Result |
| --- | --- |
| Bootloader (AM32 v19.0.0, PB4), firmware 2.21 and EEPROM on all four MCUs | Written and read back correctly over SWD (board 1); written and verified on a re-soldered chip (board 2) |
| Motor spin | A GEPRC SPEEDX2 2806.5 1350KV motor spins from an ESP32 PWM signal (no propeller, current-limited supply) |
| Bootloader on the signal wire | Replies `34 37 31 14 1F 06 06 03 30` to an ESP32 handshake at 19200 baud |
| Settings read back through the FC (ArduCopter passthrough, am32.ca) | Board 1: ESC 1, 2 and 4. Board 2: all four. Poles 14, KV 1340 |
| Motor mapping and direction on the frame | Set up and checked with Motor Test on all four motors (board 2) |
| First connection to a 6S pack | Reported successful by the operator; no propeller or hover test yet |

## Open issues

- U7 (M3) on board 1 does not answer, and its signal net reads abnormally in diode mode. Cause not found.
- USB linker (FreelyRC) does not connect with the configurators. The receive line is probably not tied to the signal pin.
- Windows Multi ESC Config Tool says "No settings found" on ESCs that am32.ca reads normally.
- The 470 uF capacitor across +BATT and GND was never confirmed (4.37 nF measured in circuit), and the board's 6S voltage rating is unchecked.
- Full power, propeller and hover tests have not been done. Low-voltage cut-off, temperature limit and current limit are still at defaults.

## Contents

- [docs/overview.md](docs/overview.md): what an ESC is, the OpenESC 30x30 board, pinout, and a summary of the work done
- [docs/bringup-and-flashing.md](docs/bringup-and-flashing.md): images, EEPROM values, OpenOCD steps and expected readbacks
- [docs/motor-mapping.md](docs/motor-mapping.md): FC parameters, am32.ca card table, spin directions
- [docs/troubleshooting.md](docs/troubleshooting.md): problems met and how they were solved
- [docs/references.md](docs/references.md): source links for the PCB design, AM32 firmware and bootloader, tools and flight controller documentation
- [firmware/README.md](firmware/README.md): where the bootloader, firmware and EEPROM values come from
- [sketches/](sketches/): ESP32 test sketches (bootloader handshake, PWM spin test)

## Safety

The board has no reverse-polarity protection and no fuse. Check polarity twice before every power-up, use a fuse and a series resistor for first power-ups, and keep propellers off until every channel has been tested.

## Notes on provenance

The exact command lines of the first flash on board 1 were not saved. The commands in the docs are the ones used on board 2. Scripts used earlier to build and check the EEPROM image (`make_eeprom.py`, `check_fw.py`) are not included yet.
