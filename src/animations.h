#ifndef ANIMATIONS_H
#define ANIMATIONS_H

#include <FastLED.h>

// --- Catalogue des fonctions d'animation ---
// Chaque fonction prend :
// - le tableau de LEDs
// - le nombre de LEDs
// - la couleur de base (si applicable)
// - l'étape actuelle (step) pour le mouvement

void anim_solid(CRGB* leds, int numLeds, CRGB color);
void anim_breathe(CRGB* leds, int numLeds, CRGB color, int step);
void anim_rainbow(CRGB* leds, int numLeds, int step);
void anim_theater_chase(CRGB* leds, int numLeds, CRGB color, int step);
void anim_scanner(CRGB* leds, int numLeds, CRGB color, int step); // Exemple style K2000

#endif
