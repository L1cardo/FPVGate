# FPVGate v1.4.1 Release Notes

**Release Date:** December 14, 2024

FPVGate v1.4.1 brings powerful new features for configuration management, race analysis, and debugging. This release focuses on reliability improvements, enhanced race history features, and better developer tools.

---

## Highlights

### SD Card Configuration Backup/Restore
Never lose your settings again! Automatic backups save your complete configuration to SD card.
- **Automatic Backup** - Every config save creates a backup on SD card
- **One-Click Restore** - Restore all settings from backup in Configuration modal
- **Complete Settings** - WiFi, RX5808, LED, pilot info, webhooks, race settings
- **Backup Location** - `/sd/config/config_backup.json`

### Device-Side Settings Storage (Config v6)
Theme and voice settings now persist on the device itself!
- **Theme Persistence** - Your selected theme is remembered across all clients
- **Voice Persistence** - Selected TTS voice saved to EEPROM
- **Multi-Client Sync** - All connected devices see the same settings
- **Automatic Migration** - Seamless upgrade from config v5

### Built-In Serial Monitor
Debug your FPVGate without connecting to USB serial console!
- **Live Web Console** - Real-time serial output in Diagnostics tab
- **Structured Logging** - Timestamped logs with DEBUG/INFO/WARNING/ERROR levels
- **Auto-Scroll** - Follow live logs with manual scroll lock
- **Color-Coded** - Visual distinction between log levels
- **100-Line Buffer** - Circular buffer keeps recent history

### 🏁 Interactive Race Timeline & Playback
Visualize and replay your races with a stunning timeline interface!
- **Visual Timeline** - Flag markers showing race start, Gate 1, laps, and race stop
- **Color-Coded Events** - Green (start), Yellow (Gate 1), Blue (laps), Red (stop)
- **Race Playback** - Replay saved races with accurate timing
- **Webhook Triggers** - Playback fires webhooks for LED controllers
- **OSD Updates** - Live broadcast to OSD during playback
- **Animated Playhead** - Shows current position during replay
- **Lap Time Indicators** - Delta times between events displayed on timeline

### Race History Improvements
Better race analysis and clearer lap information!
- **Gate 1 Separation** - Gate 1 now properly labeled (not "Lap 1")
- **Correct Lap Count** - Lap count excludes Gate 1 pass
- **Total Race Time** - Displayed in race list and details
- **Slide-Down Details** - Details appear below selected race (no more scrolling!)

### Enhanced mDNS Support
Access FPVGate with a friendly hostname!
- **Friendly URL** - Connect to `http://fpvgate.local` instead of IP address
- **Works in AP & Station** - Both WiFi modes supported
- **Service Advertisement** - Automatic device discovery on network
- **Fallback Support** - Still works with IP address if mDNS unavailable

---

## Bug Fixes

### LED Settings Persistence
**Fixed:** LEDs reverting to default color/preset on page refresh
- Split LED control functions into UI-only and device command versions
- Page load now correctly reads LED preset from device config
- Manual override flag prevents status changes from overriding user settings
- New WiFi client connections no longer change LED preset

### LED Status Override
**Fixed:** `STATUS_USER_CONNECTED` event overriding user LED preset
- Modified LED `setStatus()` to check manual override flag
- Non-critical status changes ignored when manual override active
- Race events still override LEDs (intentional behavior)

---

## Technical Details

### New Features
- **DebugLogger Class** - `lib/DEBUG/debuglogger.h` - Circular buffer logging system
- **Config v6 Schema** - Added `selectedVoice` (String) and `selectedTheme` (String)
- **Backup API** - `/config/backup`, `/config/restore`, `/config/backup/check` endpoints
- **Debug API** - `/debug/logs`, `/debug/stream`, `/debug/clear` endpoints
- **Playback API** - `/timer/playbackStart`, `/timer/playbackLap`, `/timer/playbackStop`

### Changes
- Config struct now ~220 bytes (within EEPROM limits)
- Automatic migration from config v5 to v6
- SD card structure: `/sd/config/` directory for backups
- Timeline rendering with CSS flexbox and triangle flag markers
- SSE streaming for debug logs

### Memory Usage
- **Flash:** 58.9% (1,234,193 bytes / 2,097,152 bytes)
- **RAM:** 23.5% (76,976 bytes / 327,680 bytes)

---
## 📦 What's Included

This release includes:

### ESP32-S3 Firmware Binaries:
1. **bootloader.bin** - ESP32-S3 bootloader (15 KB)
2. **partitions.bin** - Partition table (3 KB)
3. **firmware.bin** - Main application firmware v1.4.1 (1.2 MB)
4. **littlefs.bin** - Web interface filesystem with all features (1 MB)

### Desktop Application:
5. **FPVGate-v1.4.1-win32-x64.zip** - Windows desktop app (103 MB)
   - Native USB connectivity via serialport
   - Same web interface as firmware
   - Full feature parity with WiFi mode
   - Extract and run `FPVGate.exe`

### Optional:
6. **sd_card_contents.zip** - Audio files for SD card (12.7 MB)
4. **littlefs.bin** - Web interface filesystem with all features

---

## Installation

### Prerequisites
- ESP32-S3-DevKitC-1 (8MB Flash)
- Python 3.x with esptool.py installed
- USB cable

### Flashing Instructions

**Windows (PowerShell):**
```powershell
# Replace COM3 with your ESP32-S3 port
python -m esptool --chip esp32s3 --port COM3 --baud 921600 `
  --before default_reset --after hard_reset write_flash -z `
  --flash_mode dio --flash_freq 80m --flash_size 8MB `
  0x0 bootloader.bin `
  0x8000 partitions.bin `
  0x10000 firmware.bin `
  0x410000 littlefs.bin
```

**Linux/macOS:**
```bash
# Replace /dev/ttyUSB0 with your ESP32-S3 port
esptool.py --chip esp32s3 --port /dev/ttyUSB0 --baud 921600 \
  --before default_reset --after hard_reset write_flash -z \
  --flash_mode dio --flash_freq 80m --flash_size 8MB \
  0x0 bootloader.bin \
  0x8000 partitions.bin \
  0x10000 firmware.bin \
  0x410000 littlefs.bin
```

### First Boot
1. Connect to WiFi AP: `FPVGate_XXXX` (password: `fpvgate1`)
2. Open browser to `http://fpvgate.local` or `http://192.168.4.1`
3. Configure your VTx frequency and calibrate RSSI thresholds
4. Start racing!

---

##  Important Notes

### Configuration Backup
- **First Use:** Existing users should manually click "Backup Config" in System Settings after upgrade
- **SD Card Required:** Config backup requires SD card to be installed and working
- **Automatic Backups:** All future config saves will auto-backup to SD card

### Config Migration
- Config v5 → v6 migration is automatic and seamless
- No action required from users
- Older config versions will be migrated on first boot

### Known Issues
- ArduinoJson deprecation warnings during compilation (cosmetic, does not affect functionality)
- FastLED I2S driver warning on ESP32-S3 (parallel output not available, uses RMT instead)

---

## Documentation

- **Full Changelog:** [CHANGELOG.md](../../CHANGELOG.md)
- **User Guide:** [docs/USER_GUIDE.md](../../docs/USER_GUIDE.md)
- **Hardware Guide:** [docs/HARDWARE_GUIDE.md](../../docs/HARDWARE_GUIDE.md)
- **Getting Started:** [docs/GETTING_STARTED.md](../../docs/GETTING_STARTED.md)

---

## Upgrading from v1.4.0

1. Flash all four binaries using instructions above
2. Device will automatically migrate config v5 → v6
3. Your settings, races, and tracks are preserved
4. **Recommended:** Backup your config after upgrade

---

## License

MIT License - See [LICENSE](../../LICENSE) for details

---

**Support:** https://github.com/LouisHitchcock/FPVGate/issues  
**Documentation:** https://github.com/LouisHitchcock/FPVGate/tree/main/docs
