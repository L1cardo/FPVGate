#pragma once

#ifdef ESP32S3

#include <Arduino.h>
#include <FastLED.h>

#define NUM_LEDS 2

typedef enum {
    RGB_OFF,
    RGB_SOLID,
    RGB_PULSE,
    RGB_FLASH,
    RGB_COUNTDOWN
} rgb_mode_e;

typedef enum {
    STATUS_BOOTING,          // Blue pulse - system booting/AP launching
    STATUS_USER_CONNECTED,   // Green solid - user connected to web interface
    STATUS_RACE_COUNTDOWN,   // Red->Yellow->Green traffic light sequence
    STATUS_RACE_RUNNING,     // Cyan solid - race in progress
    STATUS_LAP_FLASH,        // White flash - new lap detected
    STATUS_RACE_END,         // Blue solid - race ended
    STATUS_RACE_RESET,       // Off - race reset
    STATUS_BATTERY_ALARM,    // Red flash - low battery
    STATUS_OFF
} rgb_status_e;

class RgbLed {
   public:
    void init();
    void handleRgbLed(uint32_t currentTimeMs);
    
    // Status-based methods
    void setStatus(rgb_status_e status);
    void startCountdown();  // Start 3-2-1 countdown (deprecated, now just flashes green)
    void flashGreen();      // Flash green once for race start
    void flashLap();        // Flash white for lap detection
    void flashReset();      // Flash red 3 times for race reset
    void off();
    
    // Direct color control (if needed)
    void setColor(CRGB color, rgb_mode_e mode = RGB_SOLID);
    void setBrightness(uint8_t brightness);

   private:
    CRGB leds[NUM_LEDS];
    rgb_status_e currentStatus = STATUS_OFF;
    rgb_mode_e currentMode = RGB_OFF;
    CRGB targetColor = CRGB::Black;
    
    // Animation state
    uint32_t lastUpdateMs = 0;
    uint32_t flashStartMs = 0;
    uint8_t pulseValue = 0;
    bool pulseDirection = true;
    
    // Flash parameters
    bool isFlashing = false;
    uint32_t flashDuration = 0;
    uint8_t flashCount = 0;        // For multi-flash (reset)
    uint8_t flashesRemaining = 0;  // Flashes left to do
    CRGB flashColor = CRGB::White; // Color to flash
    
    // Countdown state
    bool isCountdown = false;
    uint32_t countdownStartMs = 0;
    uint8_t countdownPhase = 0;  // 0=red, 1=yellow, 2=green, 3=done
    
    void updateAnimation(uint32_t currentTimeMs);
    void updateCountdown(uint32_t currentTimeMs);
    void applyStatus();
};

#endif // ESP32S3
