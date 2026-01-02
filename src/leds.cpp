#include "leds.h"
#include "animations.h" // On inclut notre nouvelle librairie d'effets
#include "main.h"       // Pour les constantes de PIN

// --- Config Hardware len led strip ---
#define LEN_LED_PACK_CIBLE 53
#define LEN_LED_DRAGON 30
#define LEN_LED_SALOON 40

CRGB leds_pack_cible[LEN_LED_PACK_CIBLE];
CRGB leds_dragon[LEN_LED_DRAGON];
CRGB leds_salon[LEN_LED_SALOON];


// --- Controller ---
class LedStripController {
  public:
    CRGB* leds;
    int numLeds;
    
    // État
    AnimationType currentAnim;
    CRGB baseColor;
    int animSpeed;
    unsigned long lastAnimUpdate;
    int animStep; // Compteur générique passé aux fonctions d'animation

    // Gestion Flash (inchangée, c'est de la logique pure de contrôleur)
    struct FlashEvent {
        int pixelIdx;
        unsigned long endTime;
        CRGB color;
        bool active;
    };
    static const int MAX_FLASHES = 5;
    FlashEvent flashes[MAX_FLASHES];

    void init(CRGB* ledArray, int count) {
        leds = ledArray;
        numLeds = count;
        currentAnim = ANIM_OFF;
        baseColor = CRGB::Black;
        animSpeed = 50;
        animStep = 0;
        lastAnimUpdate = 0;
        for(int i=0; i<MAX_FLASHES; i++) flashes[i].active = false;
    }

    void triggerFlash(int idx, CRGB color, int duration) {
        if (idx < 0 || idx >= numLeds) return;
        unsigned long now = millis();
        for(int i=0; i<MAX_FLASHES; i++) {
            if (!flashes[i].active || flashes[i].pixelIdx == idx) {
                flashes[i].active = true;
                flashes[i].pixelIdx = idx;
                flashes[i].color = color;
                flashes[i].endTime = now + duration;
                return;
            }
        }
    }

    void update(unsigned long now) {
        // 1. GESTION DE L'ANIMATION DE FOND
        if (now - lastAnimUpdate >= animSpeed) {
            lastAnimUpdate = now;
            animStep++; // On incrémente le compteur de temps de l'animation
            
            // C'est ici qu'on fait le LIEN vers les fichiers externes
            switch (currentAnim) {
                case ANIM_OFF:           fill_solid(leds, numLeds, CRGB::Black); break;
                case ANIM_SOLID:         anim_solid(leds, numLeds, baseColor); break;
                case ANIM_BREATHE:       anim_breathe(leds, numLeds, baseColor, animStep); break;
                case ANIM_RAINBOW:       anim_rainbow(leds, numLeds, animStep); break;
                case ANIM_THEATER_CHASE: anim_theater_chase(leds, numLeds, baseColor, animStep); break;
                case ANIM_SCANNER:       anim_scanner(leds, numLeds, baseColor, animStep); break;
            }
        }

        // 2. GESTION DES FLASHS (Overlay prioritaire)
        for(int i=0; i<MAX_FLASHES; i++) {
            if (flashes[i].active) {
                if (now > flashes[i].endTime) {
                    flashes[i].active = false;
                    // Note: Au prochain tour d'animation, le pixel reprendra sa couleur d'anim
                } else {
                    leds[flashes[i].pixelIdx] = flashes[i].color;
                }
            }
        }
    }
};

LedStripController strips[NUM_STRIPS];

// --- Implémentation Publique ---

void leds_init() {
    // Mapping Hardware FastLED
    FastLED.addLeds<WS2812B, PIN_LED_PACK_CIBLE, GRB>(leds_pack_cible, LEN_LED_PACK_CIBLE);
    FastLED.addLeds<WS2812B, PIN_LED_DRAGON,     GRB>(leds_dragon, LEN_LED_DRAGON);
    FastLED.addLeds<WS2812B, PIN_LED_SALOON,     GRB>(leds_salon, LEN_LED_SALOON);
    FastLED.setBrightness(100);

    // Init Logique
    strips[0].init(leds_pack_cible, LEN_LED_PACK_CIBLE);
    strips[1].init(leds_dragon, LEN_LED_DRAGON);
    strips[2].init(leds_salon, LEN_LED_SALOON);
}

// Public helper to set a pixel on a given strip
void leds_set_pixel(int stripId, int pixelIdx, CRGB color) {
    if (stripId < 0 || stripId >= NUM_STRIPS) return;
    LedStripController &s = strips[stripId];
    if (!s.leds) return;
    if (pixelIdx < 0 || pixelIdx >= s.numLeds) return;
    s.leds[pixelIdx] = color;
}

void leds_update() {
    unsigned long now = millis();
    for(int i=0; i<NUM_STRIPS; i++) {
        strips[i].update(now);
    }
    FastLED.show();
}

void leds_set_animation(int stripId, AnimationType anim, CRGB color, int speed) {
    if(stripId >= 0 && stripId < NUM_STRIPS) {
        strips[stripId].currentAnim = anim;
        strips[stripId].baseColor = color;
        strips[stripId].animSpeed = speed;
        strips[stripId].animStep = 0; // Reset l'animation au début
    }
}

void leds_flash_pixel(int stripId, int pixelIdx, CRGB color, int duration) {
    if(stripId >= 0 && stripId < NUM_STRIPS) {
        strips[stripId].triggerFlash(pixelIdx, color, duration);
    }
}
