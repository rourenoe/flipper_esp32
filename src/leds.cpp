#include "leds.h"
#include "main.h"

// Simple mapping of logical strips to ranges in the global `leds` array.
static const int MAX_STRIPS = 8;
static int stripStart[MAX_STRIPS];
static int stripLen[MAX_STRIPS];
static uint8_t stripPin[MAX_STRIPS];
static int stripCount = 0;

// Blink state per global LED index
static unsigned long blinkEnd[NUM_LEDS];
static CRGB savedColor[NUM_LEDS];

// Animation state (moved from main.cpp)
static int redBrightness = 0;
static int blueBrightness = 255;
static int redDirection = 3;
static int blueDirection = -1;
static unsigned long lastAnimUpdate = 0;


void register_led_strips() {
    stripCount = 0;
    // Manually construct the known strips. Adjust start/length as needed.
#ifdef LED_PACK_CIBLE
    stripPin[stripCount] = LED_PACK_CIBLE;
    stripStart[stripCount] = 0;
    stripLen[stripCount] = NUM_LEDS;
    FastLED.addLeds<WS2812B, LED_PACK_CIBLE, GRB>(leds + stripStart[stripCount], stripLen[stripCount]);
    stripCount++;
#endif

#ifdef LED_DRAGON
    stripPin[stripCount] = LED_DRAGON;
    stripStart[stripCount] = 0;
    stripLen[stripCount] = NUM_LEDS;
    FastLED.addLeds<WS2812B, LED_DRAGON, GRB>(leds + stripStart[stripCount], stripLen[stripCount]);
    stripCount++;
#endif

#ifdef LED_SALOON
    stripPin[stripCount] = LED_SALOON;
    stripStart[stripCount] = 0;
    stripLen[stripCount] = NUM_LEDS;
    FastLED.addLeds<WS2812B, LED_SALOON, GRB>(leds + stripStart[stripCount], stripLen[stripCount]);
    stripCount++;
#endif
}

void led_set_color(int stripId, int offset, CRGB color) {
    if (stripId < 0 || stripId >= stripCount) return;
    if (offset < 0 || offset >= stripLen[stripId]) return;
    int idx = stripStart[stripId] + offset;
    leds[idx] = color;
    FastLED.show();
}

void led_blink(int stripId, int offset, CRGB color, unsigned long durationMs) {
    if (stripId < 0 || stripId >= stripCount) return;
    if (offset < 0 || offset >= stripLen[stripId]) return;
    int idx = stripStart[stripId] + offset;
    // save current color
    savedColor[idx] = leds[idx];
    leds[idx] = color;
    blinkEnd[idx] = millis() + durationMs;
    FastLED.show();
}

void leds_update() {
    unsigned long now = millis();
    bool changed = false;

    // Animation update (every 5ms like original)
    if (now - lastAnimUpdate >= 5) {
        // Update red LEDs (0-39)
        for (int i = 0; i < 40 && i < NUM_LEDS; i++) {
            if (blinkEnd[i] == 0) {
                leds[i] = CRGB(redBrightness, 0, 0);
                changed = true;
            }
        }
        // Update blue LEDs (40-52)
        for (int i = 40; i < 53 && i < NUM_LEDS; i++) {
            if (blinkEnd[i] == 0) {
                leds[i] = CRGB(0, 0, blueBrightness);
                changed = true;
            }
        }
        // Update brightness values and directions
        redBrightness += redDirection;
        blueBrightness += blueDirection;
        if (redBrightness >= 255) {
            redBrightness = 255;
            redDirection = -1;
        } else if (redBrightness <= 0) {
            redBrightness = 0;
            redDirection = 1;
        }
        if (blueBrightness >= 255) {
            blueBrightness = 255;
            blueDirection = -1;
        } else if (blueBrightness <= 0) {
            blueBrightness = 0;
            blueDirection = 1;
        }
        lastAnimUpdate = now;
    }

    // Process blink expirations
    for (int i = 0; i < NUM_LEDS; ++i) {
        if (blinkEnd[i] != 0 && now >= blinkEnd[i]) {
            leds[i] = savedColor[i];
            blinkEnd[i] = 0;
            changed = true;
        }
    }

    if (changed) FastLED.show();
}
