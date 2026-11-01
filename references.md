# References and sources

Where each part of this work comes from. Links were checked on 1 November 2026 unless a note says otherwise.

## Hardware (PCB design)

| Item | Source | Licence |
| --- | --- | --- |
| OpenESC 30x30 4-in-1 ESC (KiCad design, Gerbers, BOM), designed by Incutec BV. This work used revision 3.3 / 3.4 production files | [github.com/incutec-hw/OpenESC-30x30](https://github.com/incutec-hw/OpenESC-30x30) (repository address as listed on the OSHWA certificate; not opened directly in this session) | CERN-OHL-S-2.0 (hardware and documentation) |
| OSHWA certification for OpenESC 30x30 | [certification.oshwa.org/be000029.html](https://certification.oshwa.org/be000029.html) | |
| Mirror of the same repository, with the design notes (four AT32F421 controllers running AM32, six layers, DShot on an 8-pin connector) | [hub.allspice.io/AllSpiceMirrors/incutec-OpenESC-30x30](https://hub.allspice.io/AllSpiceMirrors/incutec-OpenESC-30x30) | CERN-OHL-S-2.0 |

The netlist, pad positions and BOM used for the signal-path table in `troubleshooting.md` come from the rev 3.4 production archive of this design. The CERN-OHL-S licence is "share-alike": if this repository ever includes files derived from the design, they must carry the same licence and a notice naming Incutec BV.

## Firmware, bootloader and configurator

| Item | Source |
| --- | --- |
| AM32 firmware (version 2.21, target `OPENESC_30_F421`) | [github.com/am32-firmware/AM32](https://github.com/am32-firmware/AM32) |
| AM32 bootloader (v19.0.0, F421 build for the PB4 signal pin) | AM32-bootloader repository of the same project, `am32-firmware/AM32-bootloader` (address not opened in this session; confirm before citing) |
| am32.ca web configurator (reading and writing ESC settings through the flight controller) | [am32.ca](https://am32.ca) |

## Flight controller side

| Item | Source |
| --- | --- |
| ArduCopter DShot, BLHeli/AM32 passthrough and `SERVO_BLH_*` parameters | ArduPilot documentation, "DShot and BLHeli ESCs" pages at [ardupilot.org](https://ardupilot.org) |
| Quad X motor numbering and spin directions | ArduPilot documentation, "Connect ESCs and motors" page at [ardupilot.org](https://ardupilot.org) |
| Mission Planner (parameters and Motor Test) | [ardupilot.org/planner](https://ardupilot.org/planner) |

## Programming and test tools

| Item | Source |
| --- | --- |
| OpenOCD (xPack build 0.12.0-7) with the Artery AT32F4 target files | [xpack-dev-tools.github.io/openocd-xpack](https://xpack-dev-tools.github.io/openocd-xpack/) and [openocd.org](https://openocd.org) |
| AT32F421 microcontroller documentation (datasheet, reference manual) | Artery Technology, [arterytek.com](https://www.arterytek.com) |
| Arduino-ESP32 core, used for the `bl_test` and `pwm_test` sketches | [github.com/espressif/arduino-esp32](https://github.com/espressif/arduino-esp32) |
| Single-wire handshake format (`BLHeli` string, 19200 baud), background | [Voodoobrew101/AM32-MultiRotor-ESC-firmware wiki, Arduino PC Link](https://github.com/Voodoobrew101/AM32-MultiRotor-ESC-firmware/wiki/Arduino-PC-Link) |

## Motor and drone hardware

| Item | Source |
| --- | --- |
| GEPRC SPEEDX2 2806.5 1350KV motors | GEPRC product page (add the link you bought from) |
| Flight controller | SAL-FC GAMUDA Rev A (internal, STM32H743) |

## Notes

- Page addresses for ArduPilot, Artery and GEPRC are given at site level. Replace them with the exact page you used when you have it.
- Program and file names in this repository belong to their owners. The AM32 firmware and bootloader binaries are not stored here (see `firmware/README.md`).
