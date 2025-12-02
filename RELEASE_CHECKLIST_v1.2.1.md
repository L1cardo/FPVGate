# Release Checklist v1.2.1

## Pre-Release Verification

### Security
- [x] No API keys in code (checked with grep)
- [x] API keys only loaded from environment variables
- [x] .gitignore updated to exclude sensitive files
- [x] WARP temp files excluded from git

### Code Quality
- [x] Firmware compiles without errors
- [x] Only deprecation warnings (ArduinoJson) - acceptable
- [x] Flash usage: 49.3% (1034KB / 2097KB) ✅
- [x] RAM usage: 16.7% (54KB / 327KB) ✅

### Features Tested
- [x] SD card detection and mounting
- [x] Audio files served from SD card
- [x] Multi-voice system (4 voices: Sarah, Rachel, Adam, Antoni)
- [x] Voice switching in web interface
- [x] Graceful fallback to LittleFS if SD unavailable
- [x] Self-test enhanced with voice directory detection
- [x] Web interface loads correctly
- [x] Race timing functionality preserved

### Documentation
- [x] README.md updated with v1.2.1 features
- [x] QUICKSTART.md updated with SD card setup
- [x] RELEASE_NOTES_v1.2.1.md created
- [x] SD_CARD_MIGRATION_GUIDE.md created
- [x] MULTI_VOICE_SETUP.md created
- [x] CHANGELOG.md updated
- [x] WARP.md created for development guidance

### Files to Include in Release

#### Source Code
- All source files in `src/` and `lib/`
- `platformio.ini` and target configs
- `custom_8mb.csv` (optimized partition table)
- Web interface files in `data/`

#### Documentation
- README.md
- QUICKSTART.md
- CHANGELOG.md
- LICENSE
- RELEASE_NOTES_v1.2.1.md
- SD_CARD_MIGRATION_GUIDE.md
- MULTI_VOICE_SETUP.md
- VOICE_GENERATION_README.md

#### SD Card Content
- `sd_card/` directory with all 4 voice directories (856 files, ~12.5 MB)
- README in sd_card explaining structure

#### Scripts
- `generate_voice_files.py`
- `generate_all_voices.py`
- `requirements.txt`

### NOT to Include
- ❌ `.pio/` build artifacts
- ❌ API keys or `.env` files
- ❌ Personal voice files
- ❌ WARP temporary files
- ❌ data_full_voices/ (staging directory)

## Build Release Binaries

### Commands
```powershell
# Clean build
python -m platformio run -e ESP32S3 -t clean

# Build firmware
python -m platformio run -e ESP32S3

# Build filesystem (minimal - no sounds)
python -m platformio run -e ESP32S3 -t buildfs
```

### Copy Release Files
```powershell
# Create release directory
New-Item -ItemType Directory -Path "release/v1.2.1" -Force

# Copy binaries
Copy-Item ".pio/build/ESP32S3/bootloader.bin" "release/v1.2.1/bootloader-esp32s3.bin"
Copy-Item ".pio/build/ESP32S3/partitions.bin" "release/v1.2.1/partitions-esp32s3.bin"
Copy-Item ".pio/build/ESP32S3/firmware.bin" "release/v1.2.1/firmware-esp32s3.bin"
Copy-Item ".pio/build/ESP32S3/littlefs.bin" "release/v1.2.1/littlefs-esp32s3.bin"
```

### Package SD Card Contents
```powershell
# Zip SD card directory
Compress-Archive -Path "sd_card\*" -DestinationPath "release/v1.2.1/sd_card_voices.zip"
```

## GitHub Release Steps

1. **Tag the release**
   ```bash
   git tag -a v1.2.1 -m "v1.2.1 - SD Card Storage & Multi-Voice System"
   git push origin v1.2.1
   ```

2. **Create GitHub Release**
   - Title: `v1.2.1 - SD Card Storage & Multi-Voice System`
   - Description: Use content from RELEASE_NOTES_v1.2.1.md
   - Attach files:
     - `bootloader-esp32s3.bin`
     - `partitions-esp32s3.bin`
     - `firmware-esp32s3.bin`
     - `littlefs-esp32s3.bin`
     - `sd_card_voices.zip`
     - `FLASH_INSTRUCTIONS.txt`

3. **Update main branch**
   ```bash
   git checkout main
   git merge develop  # or your working branch
   git push origin main
   ```

## Flash Instructions for Users

Create `FLASH_INSTRUCTIONS.txt`:
```
FPVGate v1.2.1 - Flash Instructions

REQUIRED FILES:
- bootloader-esp32s3.bin
- partitions-esp32s3.bin  
- firmware-esp32s3.bin
- littlefs-esp32s3.bin
- sd_card_voices.zip (extract to SD card)

METHOD 1: Using esptool.py (All Platforms)
===========================================
esptool.py --chip esp32s3 --port COM12 --baud 460800 write_flash -z \
  0x0000 bootloader-esp32s3.bin \
  0x8000 partitions-esp32s3.bin \
  0x10000 firmware-esp32s3.bin \
  0x410000 littlefs-esp32s3.bin

Replace COM12 with your port (/dev/ttyUSB0 on Linux, /dev/cu.usbserial-* on Mac)

METHOD 2: Using ESP Flash Tool (Windows)
========================================
1. Download ESP Flash Tool from Espressif
2. Select ESP32-S3 chip
3. Add files:
   0x0000 -> bootloader-esp32s3.bin
   0x8000 -> partitions-esp32s3.bin
   0x10000 -> firmware-esp32s3.bin
   0x410000 -> littlefs-esp32s3.bin
4. Click Start

SD CARD SETUP:
==============
1. Extract sd_card_voices.zip
2. Copy all 4 folders to SD card root:
   - sounds_default (Sarah)
   - sounds_rachel (Rachel)
   - sounds_adam (Adam)
   - sounds_antoni (Antoni)
3. Insert SD card into ESP32 before powering on

DOCUMENTATION:
==============
See QUICKSTART.md for full setup guide
See MULTI_VOICE_SETUP.md for voice switching instructions
```

## Post-Release

- [ ] Test installation on clean ESP32
- [ ] Verify SD card voices work
- [ ] Update project website (if applicable)
- [ ] Announce on forums/social media
- [ ] Monitor issues for 48 hours

## Version Numbers to Update

Before release, ensure version numbers are consistent in:
- [ ] README.md (line 26 - version header)
- [ ] CHANGELOG.md (line 5 - latest version)
- [ ] RELEASE_NOTES_v1.2.1.md (line 3 - release date)

---

**Release Date:** December 1, 2024  
**Released By:** Louis Hitchcock  
**Build Status:** ✅ READY FOR RELEASE
