# Firmware, bootloader and EEPROM sources

Binary files are not stored in this repository. They are listed here with where they come from.

| File | Source | Notes |
| --- | --- | --- |
| `AM32_F421_BOOTLOADER_PB4_V19.hex` | AM32-bootloader release v19.0.0 (released 2026-08-22), asset for the F421 with the PB4 signal pin | Flashed at 0x08000000 |
| `AM32_OPENESC_30_F421_2.21.hex` | Built from AM32 source, target `OPENESC_30_F421` (the target is not in the 2.20 or 2.21 release tags, so it needed a build from master), version 2.21 | Flashed at 0x08001000. Never use the F415 hex on this board |
| `eeprom_openesc_f421_192.bin` | Built from the EEPROM values below (192 bytes) | Flashed at 0x08007C00 |

## EEPROM values

| Byte | Setting | Value |
| --- | --- | --- |
| 17 | Direction reversed | 0 (normal) |
| 25 | Startup power | 100 |
| 26 | Motor KV, stored as (KV - 20) / 40 | 33 (about 1340 KV) |
| 27 | Motor poles | 14 |
| 30 | Beeper volume | 5 |

The first 12 bytes read back from a flashed chip as the words `02000401 0001a015 6400640a`. The scripts that built and checked the image (`make_eeprom.py`, `check_fw.py`) are not included yet.
