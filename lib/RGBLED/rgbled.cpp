#include "rgbled.h"
#include "debug.h"

#define PULSE_SPEED 5
#define FLASH_DURATION_MS 200
#define COUNTDOWN_PHASE_MS 1000  // 1 second per phase (Red, Yellow, Green)

void RgbLed::init() {
    // GPIO18 for external NeoPixel strip - must be compile-time constant for FastLED template
    FastLED.addLeds<WS2812, 18, GRB>(leds, NUM_LEDS);
    FastLED.setBrightness(80); // Medium-high brightness
    // Initialize all LEDs to off
    for (int i = 0; i < NUM_LEDS; i++) {
        leds[i] = CRGB::Black;
    }
    FastLED.show();
    DEBUG("RGB LED initialized on GPIO18 with %d LEDs\n", NUM_LEDS);
}

void RgbLed::handleRgbLed(uint32_t currentTimeMs) {
    // Handle countdown sequence
    if (isCountdown) {
        updateCountdown(currentTimeMs);
        return;
    }
    
    // Handle flash timeout
    if (isFlashing && (currentTimeMs - flashStartMs) > flashDuration) {
        if (flashesRemaining > 0) {
            // More flashes to do - toggle between on and off
            flashesRemaining--;
            flashStartMs = currentTimeMs;
            CRGB color = (flashesRemaining % 2 == 0) ? flashColor : CRGB::Black;
            for (int i = 0; i < NUM_LEDS; i++) {
                leds[i] = color;
            }
            FastLED.show();
        } else {
            isFlashing = false;
            applyStatus();
        }
    }
    
    // Update animations
    updateAnimation(currentTimeMs);
}

void RgbLed::setStatus(rgb_status_e status) {
    // Don't override an active flash or countdown
    if (isFlashing || isCountdown) {
        return;
    }
    
    DEBUG("RGB LED: Setting status to %d\n", status);
    currentStatus = status;
    applyStatus();
}

void RgbLed::startCountdown() {
    // Deprecated - now just flash green and go to cyan
    flashGreen();
}

void RgbLed::flashGreen() {
    isFlashing = true;
    flashStartMs = millis();
    flashDuration = 300;  // 300ms flash
    flashesRemaining = 0; // Single flash
    flashColor = CRGB::Green;
    currentMode = RGB_SOLID;
    currentStatus = STATUS_OFF;  // Return to off after flash
    for (int i = 0; i < NUM_LEDS; i++) {
        leds[i] = flashColor;
    }
    FastLED.show();
}

void RgbLed::flashLap() {
    isFlashing = true;
    flashStartMs = millis();
    flashDuration = FLASH_DURATION_MS;
    flashesRemaining = 0; // Single flash
    flashColor = CRGB::White;
    currentMode = RGB_SOLID;
    currentStatus = STATUS_OFF;  // Return to off after flash
    for (int i = 0; i < NUM_LEDS; i++) {
        leds[i] = flashColor;
    }
    FastLED.show();
}

void RgbLed::flashReset() {
    isFlashing = true;
    flashStartMs = millis();
    flashDuration = 200;  // 200ms per flash phase
    flashesRemaining = 5; // 3 flashes = 6 phases (on-off-on-off-on-off)
    flashColor = CRGB::Red;
    currentMode = RGB_SOLID;
    currentStatus = STATUS_OFF;  // Return to off after flash
    for (int i = 0; i < NUM_LEDS; i++) {
        leds[i] = flashColor;
    }
    FastLED.show();
}

void RgbLed::off() {
    currentStatus = STATUS_OFF;
    currentMode = RGB_OFF;
    targetColor = CRGB::Black;
    isCountdown = false;
    isFlashing = false;
    for (int i = 0; i < NUM_LEDS; i++) {
        leds[i] = CRGB::Black;
    }
    FastLED.show();
}

void RgbLed::setColor(CRGB color, rgb_mode_e mode) {
    targetColor = color;
    currentMode = mode;
    if (mode == RGB_SOLID) {
        for (int i = 0; i < NUM_LEDS; i++) {
            leds[i] = color;
        }
        FastLED.show();
    }
}

void RgbLed::setBrightness(uint8_t brightness) {
    FastLED.setBrightness(brightness);
    FastLED.show();
}

void RgbLed::updateCountdown(uint32_t currentTimeMs) {
    uint32_t elapsed = currentTimeMs - countdownStartMs;
    uint32_t phaseTime = elapsed / COUNTDOWN_PHASE_MS;
    
    if (phaseTime != countdownPhase && phaseTime < 3) {
        countdownPhase = phaseTime;
        
        // Update color based on phase
        CRGB color;
        switch (countdownPhase) {
            case 0:  // RED
                color = CRGB::Red;
                break;
            case 1:  // YELLOW
                color = CRGB::Yellow;
                break;
            case 2:  // GREEN
                color = CRGB::Green;
                break;
        }
        for (int i = 0; i < NUM_LEDS; i++) {
            leds[i] = color;
        }
        FastLED.show();
    }
    
    // After 3 seconds (3 phases), countdown is done
    if (elapsed >= (COUNTDOWN_PHASE_MS * 3)) {
        isCountdown = false;
        setStatus(STATUS_RACE_RUNNING);
    }
}

void RgbLed::applyStatus() {
    switch (currentStatus) {
        case STATUS_BOOTING:
            targetColor = CRGB::Blue;
            currentMode = RGB_PULSE;
            break;
        
        case STATUS_USER_CONNECTED:
            targetColor = CRGB::Green;
            currentMode = RGB_SOLID;
            for (int i = 0; i < NUM_LEDS; i++) {
                leds[i] = targetColor;
            }
            FastLED.show();
            break;
        
        case STATUS_RACE_RUNNING:
            targetColor = CRGB::Cyan;
            currentMode = RGB_SOLID;
            for (int i = 0; i < NUM_LEDS; i++) {
                leds[i] = targetColor;
            }
            FastLED.show();
            break;
        
        case STATUS_RACE_END:
            targetColor = CRGB::Blue;
            currentMode = RGB_SOLID;
            for (int i = 0; i < NUM_LEDS; i++) {
                leds[i] = targetColor;
            }
            FastLED.show();
            break;
        
        case STATUS_BATTERY_ALARM:
            targetColor = CRGB::Red;
            currentMode = RGB_FLASH;
            break;
        
        case STATUS_RACE_RESET:
        case STATUS_OFF:
        default:
            off();
            break;
    }
}

void RgbLed::updateAnimation(uint32_t currentTimeMs) {
    // Throttle updates to reduce CPU usage
    if (currentTimeMs - lastUpdateMs < 20) {
        return;
    }
    lastUpdateMs = currentTimeMs;
    
    switch (currentMode) {
        case RGB_PULSE: {
            // Smooth pulsing effect
            if (pulseDirection) {
                pulseValue += PULSE_SPEED;
                if (pulseValue >= 255) {
                    pulseValue = 255;
                    pulseDirection = false;
                }
            } else {
                if (pulseValue < PULSE_SPEED) {
                    pulseValue = 0;
                    pulseDirection = true;
                } else {
                    pulseValue -= PULSE_SPEED;
                }
            }
            for (int i = 0; i < NUM_LEDS; i++) {
                leds[i] = targetColor;
                leds[i].nscale8(pulseValue);
            }
            FastLED.show();
            break;
        }
        
        case RGB_FLASH: {
            // Fast on/off flashing
            static bool flashState = false;
            flashState = !flashState;
            CRGB color = flashState ? targetColor : CRGB::Black;
            for (int i = 0; i < NUM_LEDS; i++) {
                leds[i] = color;
            }
            FastLED.show();
            break;
        }
        
        case RGB_SOLID:
        case RGB_OFF:
        case RGB_COUNTDOWN:
        default:
            // No animation needed
            break;
    }
}
