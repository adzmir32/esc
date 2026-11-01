# Bootloader, firmware and EEPROM process

Every MCU starts blank and needs three images, written once over SWD. After that, settings (and later firmware) can be changed over the signal wire.

| Image | File | Address | Purpose |
| --- | --- | --- | --- |
| Bootloader | `AM32_F421_BOOTLOADER_PB4_V19.hex` (AM32-bootloader v19.0.0, PB4 build) | 0x08000000, 4 KB | Waits for the configurator on the signal wire, then starts the firmware |
| Firmware | `AM32_OPENESC_30_F421_2.21.hex` (AM32 2.21, target OPENESC_30_F421) | 0x08001000 | Runs the motor |
| EEPROM | `eeprom_openesc_f421_192.bin` (192 bytes) | 0x08007C00 | Settings the firmware reads at start-up |

Use only the F421 files on this board, never the F415 hex.

EEPROM values set: poles 14 (byte 27), motor KV 1340 (byte 26 = 33, stored as (KV - 20) / 40), startup power 100 (byte 25), direction normal (byte 17 = 0), beeper volume 5 (byte 30).

## How the bootloader behaves

At power-up it checks the signal pin PB4. If the line is high it stays in the bootloader and answers the handshake (12 x 0x00, 0x0D, "BLHeli", 0xF4, 0x7D at 19200 baud) with a 9-byte reply starting `34 37 31`. If the line is low or floating it starts the firmware. The firmware has no serial handling: after about 2 seconds without a valid signal while disarmed it resets and checks the line again. So the ESC must see a high signal line at power-up to be configured, and it stays silent while waiting in the bootloader.

## Steps (once per MCU)

1. **Power and wiring.** Fit the capacitor and a series resistor bank (50 ohm, 2 x 100 ohm in parallel) with a fast-blow fuse in the + wire. Check for a short across +BATT and GND. Connect the ST-Link GND to the board GND, SWDIO to the channel's PA13 pad and SWCLK to its PA14 pad. Power from a bench supply with the motor and battery disconnected. Check 3.3 V at C66 pad 1.
2. **Set up OpenOCD** (xPack 0.12.0 with the Artery driver), in PowerShell:

```powershell
$ocd = "<path to>\openocd.exe"
$base = @("-f","interface/stlink.cfg","-c","transport select swd","-f","target/artery/at32f4x.cfg","-c","adapter speed 100")
```

3. **Identify and back up** (read-only). The log should show AT32F421G8U7 with 64 KiB of flash; a blank chip reads `ffffffff`.

```powershell
& $ocd @base -c "init" -c "halt" -c "mdw 0x08000000 4" -c "mdw 0x08001000 4" -c "mdw 0x08007c00 4" -c "shutdown"
& $ocd @base -c "init" -c "halt" -c "flash read_bank 0 <backup folder>/<name>.bin 0 65536" -c "shutdown"
```

4. **Write the three images**, each with `verify`. The hex files carry their own addresses; the EEPROM .bin needs its address. Use full paths.

```powershell
& $ocd @base -c "init" -c "halt" -c "program <path>/AM32_F421_BOOTLOADER_PB4_V19.hex verify" -c "shutdown"
& $ocd @base -c "init" -c "halt" -c "program <path>/AM32_OPENESC_30_F421_2.21.hex verify" -c "shutdown"
& $ocd @base -c "init" -c "halt" -c "program <path>/eeprom_openesc_f421_192.bin 0x08007C00 verify" -c "shutdown"
```

5. **Read back** with the identify command and compare:

| Address | Expected words |
| --- | --- |
| 0x08000000 | `20001000 0800010d 08000155 08000155` |
| 0x08001000 | `20003c00 08004279 080042c1 08004d69` |
| 0x08007c00 | `02000401 0001a015 6400640a 00000000` |

6. **Check over the signal wire.** With the ESC off, hold the signal line high with an ESP32 (`sketches/bl_test`), power the ESC, and send the handshake. A healthy bootloader replies `34 37 31 14 1F 06 06 03 30`.
7. **Read the settings** through the flight controller with am32.ca. Each ESC should show Bootloader PB4, MCU F421, EEPROM v4 and firmware OPENESC_30_F421 2.21.

## Notes

- "Verified OK" must appear after each program command.
- The OpenOCD lines "target was in unknown state when halt was requested" and "adding extra erase range" are harmless.
- Attaching the ST-Link resets the chip, so judge boot behaviour only from a cold power-up with the ST-Link removed.
- The EEPROM command showed "Verified OK" in the log on board 2. The bootloader and firmware commands were reported OK by the operator.
