# Integration Plan: ramiss/FPVGate Changes

## Author
**Richard Amiss** (@ramiss)  
Email: ramiss.android@gmail.com

## Status
- ✅ **COMPLETED**: Integration finished (5 commits integrated)
- ✅ Fixed changing frequencies (commit a18104e)
- ✅ Digital FPV band support (commits 957c86e, 041dc62)
- ✅ Improved lap timer detection (commit 911e640)
- ✅ Contributor credits and documentation (commit 0502586)
- ⚠️ **REMAINING**: Due to extensive codebase divergence, remaining bug fixes have conflicts
- 📝 **NOTE**: Remaining fixes documented below for future reference

## Changes to Integrate

### 1. RSSI Calibration Not Saving (eca4a9b) - PRIORITY HIGH
**Issue**: Calibration values not persisting to config  
**Files to modify**:
- `data/script.js` - Fix calibration save logic
- `lib/WEBSERVER/webserver.cpp` - Add endpoint to save calibration
- `src/main.cpp` - Serial logging improvements

### 2. Band/Channel Values Not Loading (bb86d82) - PRIORITY HIGH
**Issue**: Frequency band and channel not restoring from EEPROM  
**Files to modify**:
- `data/script.js` - Fix frequency loading on page load
- `lib/CONFIG/config.cpp` - Add band/channel getters
- `lib/CONFIG/config.h` - Add config fields

### 3. Config Values Not All Saving (21abcc2) - PRIORITY HIGH
**Issue**: Some config values not being written to EEPROM  
**Files to modify**:
- `lib/CONFIG/config.cpp` - Fix incomplete config save
- `lib/CONFIG/config.h` - Ensure all fields serialized
- `data/script.js` - Fix frontend config submission

### 4. Calibration UI Bugs (8165f4b, 1301994) - PRIORITY MEDIUM
**Issue**: Calibration tab UI glitches  
**Files to modify**:
- `data/index.html` - Fix calibration tab layout
- `data/script.js` - Fix calibration banner display

### 5. Self-Test Fixes (4b5dbd6) - PRIORITY MEDIUM
**Issue**: Self-tests failing or providing incorrect results  
**Files to modify**:
- `lib/SELFTEST/selftest.cpp` - Improve RX5808 test reliability
- `lib/WEBSERVER/webserver.cpp` - Fix test result formatting

### 6. Improved Lap Timer Detection (b011e05) - PRIORITY HIGH
**Issue**: False positive lap detections  
**Files to modify**:
- `lib/LAPTIMER/laptimer.cpp` - Enhanced detection algorithm
- `lib/LAPTIMER/laptimer.h` - New state machine
- `lib/KALMAN/kalman.cpp` - Kalman filter improvements

### 7. Voice Button Persistence (44c5e4e) - PRIORITY MEDIUM
**Issue**: Voice enable/disable buttons don't persist  
**Files to modify**:
- `data/script.js` - Save voice button state
- `lib/CONFIG/config.cpp` - Add voice enable fields
- `lib/CONFIG/config.h` - Config schema update

### 8. Digital Band Support (1959948) - PRIORITY HIGH
**Issue**: Only analog FPV bands supported  
**New Feature**: Support for HDZero, DJI, and other digital bands  
**Files to modify**:
- `data/script.js` - Add digital band UI
- `data/index.html` - Digital band selector
- `data/osd.js` - OSD digital band display
- `lib/CONFIG/config.cpp` - Digital frequency tables
- `lib/RX5808/RX5808.cpp` - Digital frequency support
- `src/main.cpp` - Init digital bands

### 9. Version Number Display (d1b0cfb partial) - PRIORITY LOW
**New Feature**: Show version in UI  
**Files to modify**:
- `data/index.html` - Add version display element
- `data/script.js` - Fetch and display version
- `lib/CONFIG/config.h` - Define VERSION constant
- `lib/WEBSERVER/webserver.cpp` - Add /api/version endpoint

## Implementation Approach

1. ✅ **Done**: Frequency changing fix (commit a18104e)
2. **Next**: Manually apply critical bug fixes with proper attribution
3. **Then**: Add new features (digital bands, version display)
4. **Finally**: Test thoroughly and commit with co-author credits

## Attribution
All commits will include:
```
Co-Authored-By: Richard Amiss <ramiss.android@gmail.com>
```

## Notes
- Avoid cherry-picking due to extensive conflicts with our v1.4.1 codebase
- Manual integration allows us to:
  - Skip unwanted changes (doc deletions, ESP32-C6 support)
  - Adapt code to our current structure
  - Test incrementally
  - Provide clear commit messages
