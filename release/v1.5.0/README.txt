========================================
FPVGate v1.5.0 - ESP32-S3 Firmware
========================================

Release Date: December 31, 2024
Hardware: ESP32-S3-DevKitC-1 (8MB Flash)

========================================
WHAT'S NEW IN v1.5.0
========================================

MAJOR FEATURES:
- Digital FPV band support (DJI, HDZero, WalkSnail - 16 bands total)
- Improved lap timer detection algorithm with better Kalman filtering
- Enhanced RSSI signal processing for more reliable lap detection

BUG FIXES:
- Fixed VTx frequency changes not persisting to RX5808 module
- Fixed RGB LED RMT channel conflict causing boot crashes on ESP32-S3

CONTRIBUTORS:
Special thanks to Richard Amiss (@ramiss) for digital band support,
improved lap detection, and frequency bug fixes.

========================================
INSTALLATION - FLASH FIRMWARE
========================================

PREREQUISITES:
1. ESP32-S3-DevKitC-1 board
2. USB-C cable
3. esptool.py installed (pip install esptool)
4. Python 3.7 or newer

WINDOWS FLASHING:
1. Connect ESP32-S3 via USB
2. Identify COM port (check Device Manager)
3. Open Command Prompt in this folder
4. Run the following command (replace COM3 with your port):

esptool.py --chip esp32s3 --port COM3 write_flash -z 0x0 bootloader.bin 0x8000 partitions.bin 0x10000 firmware.bin 0x410000 littlefs.bin

LINUX/MAC FLASHING:
1. Connect ESP32-S3 via USB
2. Identify port (ls /dev/tty*)
3. Open terminal in this folder
4. Run the following command (replace /dev/ttyUSB0 with your port):

esptool.py --chip esp32s3 --port /dev/ttyUSB0 write_flash -z 0x0 bootloader.bin 0x8000 partitions.bin 0x10000 firmware.bin 0x410000 littlefs.bin

ALTERNATIVE - PLATFORMIO:
If you have PlatformIO installed:
1. Place files in project root
2. Run: pio run -e ESP32S3 -t upload
3. Run: pio run -e ESP32S3 -t uploadfs

========================================
MEMORY ADDRESSES
========================================

0x0        - bootloader.bin (Boot Loader)
0x8000     - partitions.bin (Partition Table)
0x10000    - firmware.bin    (Application Firmware)
0x410000   - littlefs.bin    (Web Interface & Data)

========================================
SD CARD SETUP
========================================

REQUIRED:
- MicroSD card (1GB-32GB recommended)
- Format: FAT32
- Audio files for voice announcements

CONTENTS:
Extract sd_card_contents.zip to the root of your formatted SD card.
The structure should be:
/
├── sounds/
│   ├── sarah/
│   ├── rachel/
│   ├── adam/
│   └── antoni/
└── README.txt

Insert SD card into ESP32-S3 before first boot.

========================================
FIRST BOOT
========================================

1. Flash firmware (see above)
2. Insert SD card with audio files
3. Power cycle the device
4. Connect to WiFi AP: "FPVGate_XXXX"
5. Password: "fpvgate1"
6. Navigate to: http://www.fpvgate.xyz or http://192.168.4.1
7. Go to Configuration -> Set your VTx frequency
8. Go to Calibration -> Set RSSI thresholds
9. Start racing!

========================================
UPGRADING FROM v1.4.x
========================================

CONFIGURATION:
- Your existing config will be preserved
- LED settings will be maintained
- Race history will be preserved

CALIBRATION:
- Improved detection algorithm may require minor threshold adjustments
- Use Calibration Wizard to optimize for best performance

SD CARD:
- No changes to audio files
- Existing SD card contents are compatible

========================================
TROUBLESHOOTING
========================================

FLASH FAILED:
- Hold BOOT button while connecting USB
- Release after "Connecting..." appears
- Try a different USB cable or port

NO WIFI AP:
- Wait 30 seconds after boot
- Check serial monitor for errors
- Verify firmware flashed correctly

RGB LEDS NOT WORKING:
- This version fixes RMT channel conflicts
- Ensure external LEDs connected to GPIO5
- Check LED settings in Configuration

NO AUDIO:
- Verify SD card is properly formatted (FAT32)
- Check SD card contents match structure above
- Try different SD card if issues persist

========================================
SUPPORT & DOCUMENTATION
========================================

GitHub: https://github.com/LouisHitchcock/FPVGate
Issues: https://github.com/LouisHitchcock/FPVGate/issues
Docs:   https://github.com/LouisHitchcock/FPVGate/tree/main/docs

Full changelog: See CHANGELOG.md in repository
Release notes: See RELEASE_NOTES.md in this folder

========================================
LICENSE
========================================

FPVGate is open source software licensed under the MIT License.
See LICENSE file for details.

Made with care for the FPV community.

========================================
