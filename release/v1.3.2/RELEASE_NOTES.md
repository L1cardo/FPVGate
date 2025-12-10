# FPVGate v1.3.2 Release Notes

**Release Date:** December 10, 2025

This maintenance release adds LED settings persistence, ensuring your LED configuration survives page refreshes and device reboots.

## What's New

### WiFi Status Display
**See your connection status at a glance!** A new real-time WiFi status indicator shows exactly how your device is connected.

- **Visual status indicator** with color coding:
  -  Blue - Access Point mode (broadcasting WiFi)
  -  Green - Connected to external WiFi network
  -  Red - Connection failed or disconnected

- **Detailed information:**
  - **AP Mode:** Shows number of connected devices
  - **Station Mode:** Shows WiFi signal strength (Weak/Fair/Good/Strong)
  - Auto-updates every 5 seconds

- **Easy to spot:** Located at top of configuration page

### Marshalling Mode
**Edit race data after completion!** Perfect for correcting false triggers or missed laps.

- **Add laps:** Insert a lap time at any position in the race
  - Automatically recalculates all statistics
  - Updates race total time
  - Renumbers subsequent laps

- **Remove laps:** Delete incorrect laps (with confirmation)
  - Fixes false triggers from interference
  - Cleans up data from technical issues
  - Preserves race integrity

- **Full UI in Race History:**
  - Click race to view details
  - Editing controls appear for each lap
  - Add button to insert laps between existing ones
  - Remove button with confirmation dialog
  - All changes save automatically

- **Real-time updates:**
  - Fastest lap recalculated
  - Median lap updated
  - Best 3 laps recalculated
  - Fastest 3 consecutive recalculated
  - Charts update immediately

### LED Settings Persistence
**Your LED settings now save automatically!** No more losing your LED configuration when you refresh the page or reboot the device.

- **All LED settings persist to EEPROM:**
  - LED preset selection (Off, Solid Colour, Rainbow Wave, etc.)
  - LED brightness (0-255)
  - Animation speed (1-20)
  - Solid color
  - Fade color (for Color Fade preset)
  - Strobe color (for Strobe preset)
  - Manual override state

- **Automatic restoration on boot:** Device loads saved LED settings from EEPROM when it powers up
- **Page refresh loads current state:** Web interface reads device's current LED configuration instead of showing stale defaults

### Rainbow Wave Default
- **Rainbow Wave is now the default LED preset** - More visually appealing than the previous Solid Colour default
- First-time setup or config reset will start with Rainbow Wave effect

### Improved Race History Storage
**Better organization and reliability!** Race data now uses individual files for each race.

- **Individual race files:**
  - Each race: `/sd/races/race_<timestamp>.json`
  - Index file: `/sd/races/races_index.json`
  - Easier to manage and back up
  - Better performance with large histories

- **Automatic migration:** Old `races.json` format automatically converted
- **Corruption protection:** One corrupted file doesn't affect others
- **Works on both:** SD card and internal LittleFS storage

## Bug Fixes

### Gate 1 Timing
**Fixed!** Gate 1 lap time now correctly shows the time from race start to first gate pass. Previously, the timing calculation was incorrect, showing wrong values for the first gate crossing.

**Impact:**
- More accurate race start analysis
- Consistent timing across all laps
- Gate 1 time useful for launch technique evaluation

### Race History Not Saving
**Fixed!** Races now properly save to SD card and LittleFS. The new individual file structure ensures races are reliably stored and prevents data loss from single file corruption.

**What was wrong:**
- Race history wasn't being initialized with storage backend
- Races were lost on device reboot
- Large histories could corrupt entire file

**Now fixed:**
- Each race saves to individual file
- Index tracks all races
- Automatic migration from old format
- Better error handling

### WiFi Station Mode Connection
**Fixed!** Device now properly joins existing WiFi networks when SSID and password are configured. Connection failures correctly fall back to AP mode with visual indication.

**Improvements:**
- Credentials properly applied from config
- Connection status displayed in real-time
- Fallback to AP mode if join fails
- LED indicators show connection state

### LED Settings Reset on Page Refresh
**Fixed!** Previously, any LED changes you made would be lost when refreshing the web page. The page would revert to showing "Solid Colour" even if you had selected a different preset like Rainbow Wave. Now your settings persist properly.

**Technical Details:**
- LED settings were being sent to the device but not saved to config
- Frontend was using old ledMode (0-3) mapping instead of reading actual preset
- Config JSON buffer was too small (256 bytes) for expanded settings

## Technical Changes

### Backend
- **Config struct expanded** - Added 5 new fields: `ledPreset`, `ledSpeed`, `ledFadeColor`, `ledStrobeColor`, `ledManualOverride`
- **Config version bumped to 3** - Automatic migration from v2, resets to sensible defaults
- **Config JSON buffer increased** - 256 -> 512 bytes to handle additional fields
- **All LED endpoints save to config** - `/led/preset`, `/led/brightness`, `/led/speed`, `/led/color`, `/led/fadecolor`, `/led/strobecolor`, `/led/override`
- **Boot initialization** - `main.cpp` now loads all LED settings from config and applies them

### Frontend
- **Direct preset loading** - Reads `ledPreset` from config instead of mapping old `ledMode`
- **Comprehensive UI population** - Loads preset, brightness, speed, all colors, manual override
- **Proper UI state** - Shows/hides color pickers based on loaded preset

### Files Changed
- `lib/CONFIG/config.h` - Added LED config fields, CONFIG_VERSION = 3
- `lib/CONFIG/config.cpp` - Getters, setters, JSON serialization, defaults
- `lib/WEBSERVER/webserver.cpp` - LED endpoints now save to config
- `src/main.cpp` - Load LED config on boot
- `data/script.js` - Load LED config on page load

## What's Included

### Firmware Binaries
- `firmware-esp32s3.bin` - ESP32-S3 DevKitC-1 (main target)
- `firmware-esp32.bin` - ESP32 (PhobosLT compatible)
- `firmware-licardotimer-s3.bin` - LicardoTimer S3 variant

### Filesystem Images
- `littlefs-esp32s3.bin` - Web interface files for ESP32-S3
- `littlefs-esp32.bin` - Web interface files for ESP32
- `littlefs-licardotimer-s3.bin` - Web interface files for LicardoTimer S3

### Additional Files
- `bootloader-esp32s3.bin` - ESP32-S3 bootloader
- `partitions-esp32s3.bin` - ESP32-S3 partition table
- `sd_card_contents.zip` - Audio files for SD card (4 voice packs)

## Upgrading from v1.3.1

### Method 1: OTA Update (Recommended)
1. Connect to your FPVGate web interface
2. Go to Configuration -> System Setup
3. Click "Open Update Page"
4. Upload `firmware-esp32s3.bin`
5. Upload `littlefs-esp32s3.bin` (for web interface updates)
6. Device will reboot with new firmware

### Method 2: USB Flashing
```bash
# Flash firmware
esptool.py --chip esp32s3 --port COM12 --baud 921600 write_flash 0x10000 firmware-esp32s3.bin

# Flash filesystem
esptool.py --chip esp32s3 --port COM12 --baud 921600 write_flash 0x410000 littlefs-esp32s3.bin
```

### After Upgrade
1. **Config will be reset** - Config version bump (v2 -> v3) triggers automatic reset to defaults
2. **Reconfigure your settings:**
   - WiFi credentials (if using station mode)
   - Pilot info (name, callsign, color)
   - Race settings (min lap time, RSSI thresholds)
   - **LED settings will save automatically from now on!**
3. **Re-run calibration wizard** if needed

## Known Issues

- ESP32-C3 builds temporarily disabled due to library incompatibility
- Serial monitor may show disconnects during RGB LED operations (cosmetic, doesn't affect functionality)
- Config migration resets all settings (backup your config before upgrading)

## Coming Soon

- Per-preset color memory (remember colors for each preset)
- LED animation preview in web UI
- Config backup/restore before migration
- ESP32-C3 build fixes

## Tips

- **Use Manual Override mode** during races to prevent race events from changing LED colors
- **Export your config** (Configuration -> Download Config) before major updates
- **Adjust animation speed** to match your preference (1 = slow, 20 = fast)
- **Pilot Colour preset** automatically uses your pilot color from Pilot Info section


## Full Changelog

See [CHANGELOG.md](https://github.com/LouisHitchcock/FPVGate/blob/main/CHANGELOG.md) for complete version history.

---

**Previous Release:** [v1.3.1](https://github.com/LouisHitchcock/FPVGate/releases/tag/v1.3.1) - Race History & Calibration Fixes
