# FPVGate v1.3.0 Release Notes

**Release Date:** December 4, 2025

## What's New

### Mobile & iOS Support
- **iOS/Safari Audio Support**: Full audio functionality on iOS devices with automatic audio context unlocking
- **Mobile-Responsive Interface**: Optimized web interface for phones and tablets with adaptive layout
- **Vibration Feedback**: Race start vibration works even when phone is in silent mode
- **Touch-Optimized Controls**: Mobile-friendly buttons, navigation, and configuration options

### Cross-Device Race Storage
- **SD Card Race Storage**: Races now stored on SD card for access across all connected devices
- **Shared Race History**: View and manage races from any device connected to FPVGate
- **Automatic Directory Creation**: Creates `/sd/races/` directory on initialization
- **LittleFS Fallback**: Automatically uses internal storage if SD card unavailable

### Documentation
- **Comprehensive Wiki**: New wiki-style documentation with dedicated guides
  - Getting Started Guide
  - User Guide
  - Hardware Guide
  - Features Documentation
  - Development Guide
- **Updated README**: Clearer project overview and setup instructions

### USB Connectivity
- **Desktop Integration**: USB serial connectivity for desktop applications
- **Transport Manager**: Unified transport layer for USB and WiFi communication

## Improvements

### Audio System
- **Shared AudioContext**: Beep tones use persistent AudioContext to prevent iOS suspension
- **Audio Unlock on User Interaction**: Unlocks audio during "Enable Audio" button press
- **PiperTTS iOS Compatibility**: Web Audio API properly resumed on iOS/Safari
- **Web Speech API Loading**: Ensures voices are loaded on iOS before use

### User Interface
- **Responsive Tables**: Race history and lap tables adapt to screen size
- **Flexible Navigation**: Navigation links wrap on small screens
- **Mobile Charts**: Optimized chart heights for mobile viewing
- **Single-Column Stats**: Statistics boxes stack vertically on phones

## Bug Fixes
- Fixed PiperTTS not working on Safari/iOS due to suspended AudioContext
- Fixed race start beep tones not playing on mobile devices
- Fixed USBSerial compilation error in selftest module
- Fixed audio playback requiring multiple user interactions on iOS

## Technical Changes
- Race storage path: `/races.json` → `/sd/races/races.json`
- USBSerial references changed to Serial for ESP32-S3 compatibility
- Added iOS/Safari detection and audio unlock mechanisms
- Implemented Vibration API for race start feedback
- Added mobile-responsive CSS with media queries

## Installation

### Full Flash (Recommended for new installations)
Use `esptool.py` or ESP Flash Tool to flash:
1. `bootloader.bin` at `0x1000`
2. `partitions.bin` at `0x8000`
3. `FPVGate_v1.3.0_firmware.bin` at `0x10000`
4. `FPVGate_v1.3.0_filesystem.bin` at `0x410000`

### OTA Update (From v1.2.x)
1. Navigate to Settings > OTA Update in the web interface
2. Upload `FPVGate_v1.3.0_firmware.bin`
3. After firmware update, upload `FPVGate_v1.3.0_filesystem.bin` via LittleFS upload

### SD Card Setup
Copy the contents of `sounds_TEMP/` folder to your SD card root to enable pre-recorded voice announcements.

## Compatibility
- **ESP32-S3**: Full support with all features
- **Browsers**: Chrome, Firefox, Safari (including iOS), Edge
- **Mobile**: iOS 12+, Android 8+
- **SD Card**: FAT32 formatted (recommended 4GB or larger)

## Known Issues
- iOS silent mode still mutes audio (by design, cannot be bypassed)
- Vibration API not supported on desktop browsers (mobile only)

## Upgrade Notes
- Races stored in old LittleFS location will not automatically migrate to SD card
- Download existing race history before upgrading if you want to preserve it
- After upgrade, races will be stored on SD card if available

## Credits
Developed by the FPVGate team with community contributions.

---

For full documentation, visit: https://github.com/LouisHitchcock/FPVGate
