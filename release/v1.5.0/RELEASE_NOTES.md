# FPVGate v1.5.0 Release Notes

**Release Date:** December 31, 2024  
**Version:** 1.5.0  
**Type:** Minor Release - New Features & Bug Fixes

---

## What's New in v1.5.0

This release brings major improvements from the community, including full digital FPV system support and critical bug fixes.

### Major Features

#### Digital FPV Band Support
Full support for modern digital FPV systems with 16 new bands:

**DJI Bands:**
- v1-25, v1-25CE, v1_50
- 03/04-20, 03/04-20CE
- 03/04-40, 03/04-40CE
- 04-R

**HDZero Bands:**
- R, E, F, CE

**WalkSnail Bands:**
- R, 25, 25CE, 50

All digital bands are seamlessly integrated into the existing band selector with full frequency lookup tables.

#### Improved Lap Timer Detection Algorithm
- Significantly enhanced detection accuracy
- Minimizes false positive lap triggers
- Better Kalman filter implementation for RSSI signal processing
- More reliable detection across varying signal conditions
- Improved state machine for crossing detection

---

### Bug Fixes

#### Fixed VTx Frequency Changes Not Persisting
- Frequency updates now correctly persist to RX5808 module
- Added PIN_VBAT hardware detection in self-tests
- Improved frequency validation and error handling

#### Fixed RGB LED RMT Channel Conflict (ESP32-S3)
- Resolved boot crashes caused by RMT channel exhaustion
- Removed LED_BUILTIN (GPIO48) usage that conflicted with FastLED
- ESP32-S3 has only 4 RMT channels total - both features were competing
- External RGB LEDs (GPIO5) now function correctly without conflicts
- Device boots reliably with full RGB LED effects working

---

## Technical Details

### New Files
- `CONTRIBUTORS.md` - Community contributor credits
- `RAMISS_INTEGRATION_PLAN.md` - Integration tracking document

### Modified Files
- `data/script.js` - Added 16 digital frequency bands to lookup table
- `data/index.html` - Digital band options in band selector
- `lib/LAPTIMER/laptimer.cpp` - Improved detection algorithm (271 insertions, 93 deletions)
- `lib/LAPTIMER/laptimer.h` - Enhanced state machine
- `lib/KALMAN/kalman.cpp` - Better signal filtering
- `lib/SELFTEST/selftest.cpp` - Added PIN_VBAT conditional check
- `lib/CONFIG/config.cpp` - Frequency management improvements
- `lib/WEBSERVER/webserver.cpp` - Frequency endpoint fixes
- `lib/RX5808/RX5808.cpp` - Module communication improvements
- `lib/RGBLED/rgbled.cpp` - Removed try-catch wrapper, simplified init
- `src/main.cpp` - Removed LED_BUILTIN usage to free RMT channel
- `README.md` - Digital bands and contributor credits

---

## Contributors

Special thanks to **Richard Amiss** (@ramiss) for:
- Digital FPV band support implementation
- Improved lap timer detection algorithm
- Frequency changing bug fixes
- Kalman filter improvements

GitHub: https://github.com/ramiss/FPVGate  
Integrated: December 2024

All integrated commits include proper co-author attribution.

---

## Installation

### Option A: Flash Prebuilt Binaries (Recommended)

1. Download the release binaries from the [releases page](https://github.com/LouisHitchcock/FPVGate/releases/tag/v1.5.0)
2. Connect your ESP32-S3 via USB
3. Flash using esptool.py:

```bash
esptool.py --chip esp32s3 --port COM3 write_flash -z \
  0x0 bootloader.bin \
  0x8000 partitions.bin \
  0x10000 firmware.bin \
  0x410000 littlefs.bin
```

Replace `COM3` with your actual COM port (Windows) or `/dev/ttyUSB0` (Linux/Mac).

### Option B: Build from Source

```bash
git clone https://github.com/LouisHitchcock/FPVGate.git
cd FPVGate
git checkout v1.5.0
pio run -e ESP32S3 -t upload
pio run -e ESP32S3 -t uploadfs
```

---

## Important Notes

### Breaking Changes
None - this release is fully backwards compatible with v1.4.x configurations.

### Known Issues
- ArduinoJson deprecation warnings (non-critical, will be addressed in v1.6.0)

### Upgrade Path
If upgrading from v1.4.x:
1. Flash new firmware
2. Flash new filesystem (contains digital band data)
3. Your existing config will be preserved
4. Calibration may need minor adjustment for improved detection algorithm

---

## Links

- **GitHub Repository:** https://github.com/LouisHitchcock/FPVGate
- **Documentation:** https://github.com/LouisHitchcock/FPVGate/tree/main/docs
- **Issue Tracker:** https://github.com/LouisHitchcock/FPVGate/issues
- **Discussions:** https://github.com/LouisHitchcock/FPVGate/discussions

---

## Full Changelog

See [CHANGELOG.md](../../CHANGELOG.md#150---2024-12-31) for complete version history.

---

**Made with care for the FPV community**  
FPVGate is open source (MIT License) and built by pilots, for pilots.
