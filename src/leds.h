#ifndef LEDS_H
#define LEDS_H

#include <FastLED.h>

// Liste des IDs d'animations disponibles
// Doit correspondre aux fonctions dans animations.cpp
enum AnimationType {
    ANIM_OFF,
    ANIM_SOLID,
    ANIM_BREATHE,
    ANIM_RAINBOW,
    ANIM_THEATER_CHASE,
    ANIM_SCANNER
};

// IDs logiques des bandeaux
#define PIN_LED_PACK_CIBLE 4
#define PIN_LED_DRAGON 13
#define PIN_LED_SALOON 19

#define STRIP_PACK_CIBLE 0
#define STRIP_DRAGON 1
#define STRIP_SALOON 2

#define NUM_STRIPS 3

// Initialisation globale
void leds_init();

// Update global (à appeler dans loop)
void leds_update();

// Commandes
void leds_set_animation(int stripId, AnimationType anim, CRGB color, int speed = 50);
void leds_flash_pixel(int stripId, int pixelIdx, CRGB color, int duration = 200);
// Set a single pixel on a given strip (safe bounds-checked)
void leds_set_pixel(int stripId, int pixelIdx, CRGB color);

#endif
