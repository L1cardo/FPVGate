# WARP.md

This file provides guidance to WARP (warp.dev) when working with code in this repository.

## Project Overview

FPVGate is a personal FPV lap timer for ESP32-S3 that uses RSSI-based timing to detect drone passes through a virtual gate. It features RGB LED indicators, a modern web interface with 23 themes, dual TTS systems (ElevenLabs pre-recorded + PiperTTS), and race history tracking.

**Key Technologies:**
- Platform: PlatformIO + Arduino framework for ESP32-S3
- Hardware: ESP32-S3-DevKitC-1 + RX5808 5.8GHz receiver module
- Web: AsyncWebServer with Server-Sent Events (SSE)
- Storage: LittleFS filesystem + EEPROM for config
- Audio: Pre-recorded MP3 files (ElevenLabs TTS) + PiperTTS fallback
- LEDs: FastLED library for WS2812 RGB control

## Common Development Commands

### Building & Flashing
```powershell
# Build firmware for ESP32-S3
pio run -e ESP32S3

# Flash firmware to device
pio run -e ESP32S3 -t upload

# Upload web interface files (LittleFS filesystem)
pio run -e ESP32S3 -t uploadfs

# Clean build artifacts
pio run -e ESP32S3 -t clean

# Monitor serial output
pio device monitor -e ESP32S3
```

### Voice Generation (Python)
```powershell
# Install Python dependencies
pip install -r requirements.txt

# Generate voice files (requires ELEVENLABS_API_KEY environment variable)
python generate_voice_files.py

# Generate all voices (Sarah, Rachel, Adam, Antoni)
python generate_all_voices.py

# Auto-generate with defaults
python generate_voice_files_auto.py

# Regenerate audio files
python regenerate_audio.py
```

### Testing
This project does not have automated tests. Hardware functionality should be verified using:
- The System Self-Test feature in Configuration tab (web UI)
- Manual testing of lap detection and timing
- RSSI calibration via the Calibration tab

## Architecture Overview

### Core System Design

FPVGate uses a **dual-core architecture** leveraging ESP32-S3's capabilities:

**Core 1 (main loop):**
- `LapTimer::handleLapTimerUpdate()` - High-priority RSSI sampling and lap detection
- `ElegantOTA::loop()` - OTA update handling

**Core 0 (parallel task):**
- Buzzer, LED, and RGB LED animations
- Web server event handling
- EEPROM configuration persistence
- RX5808 frequency changes
- Battery monitoring

This separation ensures timing-critical RSSI measurements are not interrupted by I/O operations.

### Module Structure

The codebase is organized into self-contained libraries in `lib/`:

**Hardware Interface Layers:**
- `RX5808/` - SPI communication with RX5808 video receiver module, RSSI reading
- `RGBLED/` - FastLED-based WS2812 RGB LED control with 10 visual presets
- `LED/` - Simple onboard LED control
- `BUZZER/` - Active buzzer audio feedback
- `BATTERY/` - ADC-based voltage monitoring with low battery alerts

**Core Business Logic:**
- `LAPTIMER/` - RSSI-based lap detection state machine using Kalman filtering
  - States: STOPPED, WAITING (for hole shot), RUNNING, CALIBRATION_WIZARD
  - Peak detection algorithm: Enter threshold → Peak capture → Exit threshold
- `CONFIG/` - EEPROM-backed configuration with auto-save
- `RACEHISTORY/` - Persistent race session storage in JSON format
- `STORAGE/` - LittleFS filesystem abstraction

**Supporting Systems:**
- `WEBSERVER/` - WiFi AP/STA mode, AsyncWebServer, SSE events, captive portal
- `KALMAN/` - Kalman filter for RSSI noise reduction
- `SELFTEST/` - Comprehensive hardware/software diagnostics (12 tests)
- `DEBUG/` - Serial debug macros

### Lap Detection Algorithm

The lap timer uses a **peak-capture algorithm** with hysteresis:

1. **Idle**: RSSI below Enter threshold
2. **Capturing**: RSSI rises above Enter threshold → track peak RSSI value
3. **Peak Found**: RSSI falls below Exit threshold → record lap time
4. **Minimum Lap Protection**: Ignore peaks within configurable minimum lap time (default 5s)

The Kalman filter smooths RSSI readings to prevent noise-induced false triggers. Thresholds must be calibrated per environment (see Calibration tab).

### Web Architecture

**Frontend** (`data/` directory):
- `index.html` - Single-page app with 4 tabs: Race, Calibration, Configuration, Race History
- `script.js` - SSE event handling, race control logic, lap analysis with Chart.js
- `style.css` - 23 Material Design themes with dynamic SVG logo color adaptation
- `audio-announcer.js` - Dual TTS system: pre-recorded MP3 → PiperTTS fallback
- `smoothie.js` - Real-time RSSI graph visualization

**Backend** (`lib/WEBSERVER/`):
- REST-like JSON API endpoints for config, race control, calibration
- Server-Sent Events (SSE) for real-time RSSI streaming and lap notifications
- Captive portal for automatic web interface redirection
- OTA update integration via ElegantOTA library

### Configuration System

Settings are stored in EEPROM with a versioned struct (`laptimer_config_t`). Changes trigger auto-save after 1 second of inactivity. The config includes:
- VTx frequency (band/channel)
- RSSI thresholds (enter/exit)
- Race settings (min lap time, max laps)
- LED mode, brightness, custom color
- Announcer type, rate, voice selection
- Pilot info (name, callsign, phonetic name, color)

Frontend settings (theme, audio preferences) are stored in browser LocalStorage and exported alongside backend config for complete backups.

## Key File Locations

### Source Code
- `src/main.cpp` - Entry point, hardware initialization, dual-core task setup
- `platformio.ini` - Build configuration (imports target configs)
- `targets/ESP32S3.ini` - ESP32-S3 specific settings, dependencies, build flags

### Pin Definitions
- `lib/CONFIG/config.h` - Hardware pin mappings (search for `#elif defined(ESP32S3)`)
  - GPIO4: RX5808 RSSI (never use GPIO3 - strapping pin!)
  - GPIO10-12: RX5808 SPI (DATA, SELECT, CLOCK)
  - GPIO48: WS2812 RGB LED (external, not onboard)
  - GPIO5: Buzzer
  - GPIO1: Battery voltage sense

### Web Interface
- `data/index.html` - Web UI layout
- `data/script.js` - Frontend race logic, SSE handling
- `data/audio-announcer.js` - TTS engine with MP3 file mapping

### Documentation
- `README.md` - Complete user documentation
- `QUICKSTART.md` - Quick start guide for new users
- `VOICE_GENERATION_README.md` - Voice file generation guide
- `CHANGELOG.md` - Version history

## Important Development Notes

### Hardware Constraints
- **GPIO3 Warning**: Never connect anything to GPIO3 on ESP32-S3 - it's a strapping pin and will prevent firmware flashing
- **5V Power**: Both ESP32-S3 and RX5808 require 5V via VBUS pin
- **RX5808 SPI Mod**: The RX5808 module must be SPI-modded (see README for guide)
- **LED Pin**: GPIO48 is for external WS2812 strips (1-2 LEDs). Edit `NUM_LEDS` in `lib/RGBLED/rgbled.h` if using different count

### Code Patterns
- Use `DEBUG()` macro from `debug.h` for serial logging
- RSSI values are `uint8_t` (0-255) after Kalman filtering
- All times are in milliseconds (`uint32_t currentTimeMs`)
- RGB LED access requires ESP32S3 build flag and global pointer check: `#ifdef ESP32S3` + `if (g_rgbLed)`
- Config changes call `config.write()` which triggers deferred EEPROM save

### Audio System
The dual TTS system tries pre-recorded MP3 files first, falls back to PiperTTS:
- **ElevenLabs voices** (Sarah/Rachel/Adam/Antoni): Pre-recorded, 1.3x playback speed
- **PiperTTS**: Exclusive mode (no pre-recorded files), lower latency
- Numbers use natural pronunciation: "11.44" = "eleven point forty-four" (not "one one point four four")
- Voice files generated via `generate_voice_files.py` (requires ElevenLabs API key)

### Filesystem Upload Warning
**Race history is erased when running `uploadfs`** because it overwrites the entire LittleFS partition. Always download race history backups before reflashing the filesystem.

### Build Targets
- `ESP32S3` - Primary target (ESP32-S3-DevKitC-1)
- `ESP32C3` - Alternative ESP32-C3 target
- `PhobosLT` - Legacy PhobosLT hardware target
- Always use `-e ESP32S3` flag for this project

## Calibration Best Practices

RSSI threshold calibration is critical for accurate timing:
1. Power on drone and wait 30 seconds for VTx thermal stabilization
2. Place drone at gate distance (3-4 feet for indoor, 6-8 feet outdoor)
3. Observe RSSI peak value on Calibration tab graph
4. Set Enter RSSI: 2-5 points below peak
5. Set Exit RSSI: 8-10 points below Enter
6. Test with practice laps and adjust if false triggers occur

If flying with other pilots nearby (RF interference), lower both thresholds by 3-5 points.
