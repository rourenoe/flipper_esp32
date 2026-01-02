#include "animations.h"

// 1. Animation Solide (Fixe)
void anim_solid(CRGB* leds, int numLeds, CRGB color) {
    fill_solid(leds, numLeds, color);
}

// 2. Animation Respiration (Fade In / Fade Out)
void anim_breathe(CRGB* leds, int numLeds, CRGB color, int step) {
    // Utilise step pour créer une onde sinusoïdale douce
    // step évolue à chaque frame définie par la vitesse
    float val = (exp(sin(step / 20.0)) - 0.36787944) * 108.0;
    
    CRGB c = color;
    c.nscale8(val); // Applique la luminosité calculée
    fill_solid(leds, numLeds, c);
}

// 3. Animation Arc-en-ciel
void anim_rainbow(CRGB* leds, int numLeds, int step) {
    // step définit le décalage de la teinte de départ
    // 7 est le "delta hue" entre chaque led (densité de l'arc en ciel)
    fill_rainbow(leds, numLeds, step, 7);
}

// 4. Animation Theater Chase (Chenillard)
void anim_theater_chase(CRGB* leds, int numLeds, CRGB color, int step) {
    fill_solid(leds, numLeds, CRGB::Black);
    int offset = step % 3;
    for (int i = offset; i < numLeds; i += 3) {
        leds[i] = color;
    }
}

// 5. Animation Scanner (K2000 / Cylon)
void anim_scanner(CRGB* leds, int numLeds, CRGB color, int step) {
    // Effet d'effacement progressif (trail)
    fadeToBlackBy(leds, numLeds, 20);
    
    // Calcul de la position du point lumineux (aller-retour)
    // On utilise une fonction ping-pong mathématique simple
    int pos = beatsin16(30, 0, numLeds - 1); 
    leds[pos] = color;
}
