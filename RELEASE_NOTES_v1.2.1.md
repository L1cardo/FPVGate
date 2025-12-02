# FPVGate v1.2.1 - SD Card Storage Expansion

**Release Date:** December 1, 2024

## 🎉 Major Feature: SD Card Storage

FPVGate now supports **MicroSD card storage**, dramatically expanding capacity and freeing up precious ESP32 flash memory!

### What's New

#### SD Card Audio Storage
- **Audio files moved to SD card** - All voice files now stored on SD card
- **4 pre-generated voices** - Sarah, Rachel, Adam, Antoni (214 files each, ~12.5 MB total)
- **Instant voice switching** - Change voices in real-time via web interface
- **85% more free flash space** - LittleFS reduced from 4.4MB to 1MB
- **Automatic migration** - System auto-copies sounds from flash to SD on first boot
- **Seamless streaming** - Audio served directly from SD with zero latency impact
- **Graceful fallback** - If SD card unavailable, system continues from LittleFS

#### Multi-Voice System
- **4 ElevenLabs voices included** - Sarah (Energetic), Rachel (Calm), Adam (Deep Male), Antoni (Male)
- **Dynamic voice selection** - Choose voice in Configuration tab, instantly active
- **Voice-specific directories** - Each voice in separate SD folder (`sounds_default`, `sounds_rachel`, etc.)
- **Smart caching** - Audio cached in browser, cleared on voice change
- **No performance impact** - Voice switching doesn't affect race timing

#### Expanded OTA Capacity
- **2MB firmware support** (vs 1.8MB before)
- **Larger app partitions** - Each OTA slot now 2MB instead of 1.8MB
- **Room for growth** - Add more features without storage constraints

#### Optimized Partition Table
```
Old Layout (4.4MB LittleFS):
├── app0:   1.8MB (firmware slot 1)
├── app1:   1.8MB (firmware slot 2)
└── spiffs: 4.4MB (web UI + audio)

New Layout (1MB LittleFS + SD):
├── app0:   2.0MB (firmware slot 1) ✅ +200KB
├── app1:   2.0MB (firmware slot 2) ✅ +200KB
├── spiffs: 1.0MB (web UI only)    ✅ -3.4MB freed
└── SD:     Unlimited (audio files) 🎵
```

### Hardware Requirements

#### New: MicroSD Card Module
- **Pins:** GPIO39 (CS), GPIO36 (SCK), GPIO35 (MOSI), GPIO37 (MISO)
- **Format:** FAT32
- **Size:** 1GB minimum (any SDHC/SDXC card works)
- **Wiring:** 3.3V power, common ground

### Setup Instructions

#### Option 1: Pre-populate SD Card (Recommended)
1. Format SD card as FAT32
2. Copy `data/sounds/` directory to SD card root
3. Insert SD card into ESP32
4. Flash firmware normally

#### Option 2: Automatic Migration
1. Flash firmware with SD card inserted
2. System auto-copies sounds from LittleFS → SD after 5 seconds
3. (Optional) Re-flash minimal LittleFS to reclaim space

See **SD_CARD_MIGRATION_GUIDE.md** for detailed instructions.

### Technical Details

#### Pin Configuration
| ESP32-S3 GPIO | SD Card Pin |
|---------------|-------------|
| GPIO39 | CS (Chip Select) |
| GPIO36 | SCK (Clock) |
| GPIO35 | MOSI (Data In) |
| GPIO37 | MISO (Data Out) |
| 3.3V | VCC |
| GND | GND |

#### File Serving Logic
1. Check if file exists on **SD card** (if available)
2. Fall back to **LittleFS** (always available)
3. Return 404 if not found in either location

#### Storage Class API
- `Storage::isSDAvailable()` - Check if SD card mounted
- `Storage::migrateSoundsToSD()` - Copy sounds from flash to SD
- `Storage::getStorageType()` - Returns "SD" or "LittleFS"

### Benefits

✅ **85% more free flash** - Massive space savings  
✅ **Unlimited audio expansion** - Add voices without reflashing  
✅ **Faster OTA updates** - Larger firmware capacity  
✅ **Future-proof** - Store telemetry, logs, or race data on SD  
✅ **No performance impact** - Audio streams directly, no buffering  
✅ **Robust fallback** - System works without SD card

### Compatibility

- **Requires:** ESP32-S3-DevKitC-1
- **SD Card:** Optional but highly recommended
- **Backward compatible:** Works without SD card (uses LittleFS)
- **Existing setups:** Can add SD card to any existing FPVGate

### Migration Notes

#### From v1.2.0 → v1.2.1

**If you have SD card:**
1. Flash v1.2.1 firmware
2. Copy sounds to SD card
3. Enjoy 85% more free space!

**Without SD card:**
- Everything works as before
- Audio served from LittleFS
- No changes required

#### Partition Table Changed
- **Important:** OTA updates use new 2MB partitions
- First flash after upgrade must use USB (not OTA)
- Subsequent OTA updates work normally

### Known Issues

- None reported

### Testing Performed

✅ SD card detection and mounting  
✅ Audio streaming from SD card  
✅ Automatic migration from LittleFS  
✅ Fallback to LittleFS when SD unavailable  
✅ Web interface functionality unchanged  
✅ Race history saves to appropriate storage  
✅ OTA updates with new partition table  
✅ Serial monitor diagnostics  

### Upgrade Path

1. **Build firmware:**
   ```powershell
   python -m platformio run -e ESP32S3
   ```

2. **Flash via USB:**
   ```powershell
   python -m platformio run -e ESP32S3 -t upload --upload-port COM12
   ```

3. **Prepare SD card** (see SD_CARD_MIGRATION_GUIDE.md)

4. **Upload filesystem** (optional - only if you want sounds in LittleFS too):
   ```powershell
   python -m platformio run -e ESP32S3 -t uploadfs --upload-port COM12
   ```

### Files Changed

- `lib/CONFIG/config.h` - Updated SD pin definitions
- `lib/STORAGE/storage.cpp` - Enabled SD card initialization
- `lib/STORAGE/storage.h` - Added migration methods
- `lib/WEBSERVER/webserver.cpp` - Added SD-aware audio streaming
- `src/main.cpp` - Added deferred SD init and migration trigger
- `custom_8mb.csv` - Optimized partition table
- `README.md` - Updated documentation
- `QUICKSTART.md` - Added SD card setup guide
- `WARP.md` - Updated architecture notes

### Credits

SD card implementation inspired by ESP32 SD examples and optimized for FPVGate's audio streaming requirements.

### Support

- **Documentation:** SD_CARD_MIGRATION_GUIDE.md
- **Issues:** https://github.com/LouisHitchcock/FPVGate/issues
- **Discussions:** https://github.com/LouisHitchcock/FPVGate/discussions

---

**Full Changelog:** v1.2.0...v1.2.1
