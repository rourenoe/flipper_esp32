#ifndef LEDS_H
#define LEDS_H
#include <Arduino.h>
#include <FastLED.h>

// Register a logical LED strip mapping to the shared `leds` buffer.
// `pin` is only informational here; FastLED must already be configured for the underlying LEDs.
// Returns a stripId (>=0) or -1 on error.
// Register physical strips with FastLED using compile-time constants.
// This function does not take arguments; it registers the known constant strips.
void register_led_strips();

// Set a color for a given LED within a logical strip (immediate show).
void led_set_color(int stripId, int offset, CRGB color);

// Blink a LED on a strip for `durationMs` milliseconds (non-blocking).
void led_blink(int stripId, int offset, CRGB color, unsigned long durationMs);

// Call regularly from loop() to process timed blinks and animations.
void leds_update();

#endif // LEDS_H
