===============================================================================
  FPVGate v1.4.1 - ESP32-S3 Lap Timer
===============================================================================

Release Date: December 14, 2024

WHAT'S INCLUDED:
  ESP32-S3 Firmware:
  - bootloader.bin                  (ESP32-S3 bootloader, 15 KB)
  - partitions.bin                  (Partition table, 3 KB)
  - firmware.bin                    (Main application v1.4.1, 1.2 MB)
  - littlefs.bin                    (Web interface filesystem, 1 MB)
  
  Desktop Application:
  - FPVGate-v1.4.1-win32-x64.zip    (Windows desktop app, 103 MB)
  
  Optional:
  - sd_card_contents.zip            (Audio files for SD card, 12.7 MB)
  
  Documentation:
  - README.txt                      (This file)
  - RELEASE_NOTES.md                (Detailed release notes)

===============================================================================
  QUICK FLASH INSTRUCTIONS
===============================================================================

PREREQUISITES:
  - ESP32-S3-DevKitC-1 (8MB Flash)
  - Python 3.x with esptool.py installed
  - USB cable

WINDOWS (PowerShell):
---------------------
python -m esptool --chip esp32s3 --port COM3 --baud 921600 ^
  --before default_reset --after hard_reset write_flash -z ^
  --flash_mode dio --flash_freq 80m --flash_size 8MB ^
  0x0 bootloader.bin ^
  0x8000 partitions.bin ^
  0x10000 firmware.bin ^
  0x410000 littlefs.bin

NOTE: Replace COM3 with your actual COM port

LINUX/macOS:
------------
esptool.py --chip esp32s3 --port /dev/ttyUSB0 --baud 921600 \
  --before default_reset --after hard_reset write_flash -z \
  --flash_mode dio --flash_freq 80m --flash_size 8MB \
  0x0 bootloader.bin \
  0x8000 partitions.bin \
  0x10000 firmware.bin \
  0x410000 littlefs.bin

NOTE: Replace /dev/ttyUSB0 with your actual port

===============================================================================
  FIRST BOOT
===============================================================================

1. Connect to WiFi AP: "FPVGate_XXXX" (password: fpvgate1)
2. Open browser to http://fpvgate.local or http://192.168.4.1
3. Go to Configuration → Set your VTx frequency
4. Go to Calibration → Set RSSI thresholds
5. Start racing!

===============================================================================
  NEW IN v1.4.1
===============================================================================

MAJOR FEATURES:
  ✓ SD Card config backup/restore system
  ✓ Device-side theme & voice storage (config v6)
  ✓ Built-in serial monitor with live web console
  ✓ Enhanced mDNS support (fpvgate.local)
  ✓ Interactive race timeline with playback
  ✓ Race playback with webhook triggers
  ✓ Gate 1 handling improvements
  ✓ Total race time display
  ✓ LED persistence bug fixes

BUG FIXES:
  ✓ LEDs no longer revert to default on page refresh
  ✓ LED status override fixed

See RELEASE_NOTES.md for full details.

===============================================================================
  UPGRADING FROM v1.4.0
===============================================================================

1. Flash all four binaries using instructions above
2. Config v5 → v6 migration is automatic
3. Your settings, races, and tracks are preserved
4. RECOMMENDED: Backup your config after upgrade

===============================================================================
  SUPPORT & DOCUMENTATION
===============================================================================

GitHub:        https://github.com/LouisHitchcock/FPVGate
Issues:        https://github.com/LouisHitchcock/FPVGate/issues
Documentation: https://github.com/LouisHitchcock/FPVGate/tree/main/docs

Full Release Notes: See RELEASE_NOTES.md
Changelog:          See ../../CHANGELOG.md

===============================================================================
  LICENSE
===============================================================================

MIT License - See https://github.com/LouisHitchcock/FPVGate/blob/main/LICENSE

===============================================================================
