#include "main.h"
#include "leds.h"
// Initialize pins and interrupts for target detection

// Paramètres de configuration des LEDs

volatile bool triggerDetectedBumpBottomLeft = false;
volatile bool triggerDetectedBumpBottomRight = false;
volatile bool triggerDetectedBumpTop = false;
volatile bool triggerDetectedRmpHill = false;
volatile bool triggerDetectedRmpExt = false;

// Target detection flags
volatile bool triggerDetectedCiblePack1 = false;
volatile bool triggerDetectedCiblePack2 = false;
volatile bool triggerDetectedCiblePack3 = false;
volatile bool triggerDetectedCibleCowboyTop = false;

unsigned long timerDelayHillStart = 0; // Pour compter les 3 secondes
bool isWaitingForHill = false;

void IRAM_ATTR onTriggerRmpExt() {
    triggerDetectedRmpExt = true;
}

void IRAM_ATTR onTriggerCiblePack1() {
    triggerDetectedCiblePack1 = true;
}

void IRAM_ATTR onTriggerCiblePack2() {
    triggerDetectedCiblePack2 = true;
}

void IRAM_ATTR onTriggerCiblePack3() {
    triggerDetectedCiblePack3 = true;
}

void IRAM_ATTR onTriggerCibleCowboyTop() {
    triggerDetectedCibleCowboyTop = true;
}

void IRAM_ATTR onTriggerBumpBottomLeft() {
    triggerDetectedBumpBottomLeft = true;
}

void IRAM_ATTR onTriggerRmpHill() {
    triggerDetectedRmpHill = true;
}

void IRAM_ATTR onTriggerBumpBottomRight() {
    triggerDetectedBumpBottomRight = true;
}
void IRAM_ATTR onTriggerBumpTop() {
    triggerDetectedBumpTop = true;
}


void setup() {
  //AUTOSTART______________________
    pinMode(PC_START, OUTPUT);
    digitalWrite(PC_START, HIGH);
    delay(100);
    digitalWrite(PC_START, LOW);
    pinMode(PC_START, INPUT);
    digitalWrite(PC_START, LOW);
    pinMode(SEL_HILL, OUTPUT);
    pinMode(SEL_BUMP_BOTTOM_LEFT, OUTPUT);
    pinMode(SEL_BUMP_BOTTOM_RIGHT, OUTPUT);
    pinMode(SEL_BUMP_TOP, OUTPUT);


 //Detection targets setup
    pinMode(DETECTION_PACK_CIBLE_1, INPUT_PULLDOWN);
    attachInterrupt(digitalPinToInterrupt(DETECTION_PACK_CIBLE_1), onTriggerCiblePack1, RISING);

    pinMode(DETECTION_PACK_CIBLE_2, INPUT_PULLDOWN);
    attachInterrupt(digitalPinToInterrupt(DETECTION_PACK_CIBLE_2), onTriggerCiblePack2, RISING);

    pinMode(DETECTION_PACK_CIBLE_3, INPUT_PULLDOWN);
    attachInterrupt(digitalPinToInterrupt(DETECTION_PACK_CIBLE_3), onTriggerCiblePack3, RISING);

    pinMode(DETECTION_CIBLE_COWBOY_TOP, INPUT_PULLDOWN);
    attachInterrupt(digitalPinToInterrupt(DETECTION_CIBLE_COWBOY_TOP), onTriggerCibleCowboyTop, RISING);
  // Setup interrupt for trigger SEL_HILL
    pinMode(RMP_HILL, INPUT_PULLDOWN);
    attachInterrupt(digitalPinToInterrupt(RMP_HILL), onTriggerRmpHill, RISING);
    

    pinMode(DETECTION_RMP_EXT, INPUT_PULLDOWN);
    attachInterrupt(digitalPinToInterrupt(DETECTION_RMP_EXT), onTriggerRmpExt, RISING);

    pinMode(BUMP_BOTTOM_LEFT, INPUT_PULLDOWN);
    attachInterrupt(digitalPinToInterrupt(BUMP_BOTTOM_LEFT), onTriggerBumpBottomLeft, RISING);

    pinMode(BUMP_BOTTOM_RIGHT, INPUT_PULLDOWN);
    attachInterrupt(digitalPinToInterrupt(BUMP_BOTTOM_RIGHT), onTriggerBumpBottomRight, RISING);
    pinMode(BUMP_TOP, INPUT_PULLDOWN);
    attachInterrupt(digitalPinToInterrupt(BUMP_TOP), onTriggerBumpTop, RISING);

  FastLED.setBrightness(100);
  Serial.begin(115200);
  // Initialize LED strips before setting pixels
  leds_init();
  for (int i = 0; i < 40; i++) {
        leds_set_pixel(STRIP_PACK_CIBLE, i, CRGB::Red);
    }
    for (int i = 40; i < 53; i++) {
        leds_set_pixel(STRIP_PACK_CIBLE, i, CRGB::Blue);
    }
    FastLED.show();
    leds_set_animation(STRIP_PACK_CIBLE, ANIM_SCANNER, CRGB::Red);
    leds_set_animation(STRIP_DRAGON, ANIM_BREATHE, CRGB::Green, 20);
    leds_set_animation(STRIP_SALOON, ANIM_THEATER_CHASE, CRGB::Gold, 10);
    
}

// Non-blocking timers for triggers
unsigned long triggerTimerHill = 0;
bool triggerActiveHill = false;

unsigned long bumpBottomLeftTimer = 0;
bool bumpBottomLeftActive = false;
unsigned long bumpBottomRightTimer = 0;
bool bumpBottomRightActive = false;
unsigned long bumpTopTimer = 0;
bool bumpTopActive = false;



// Cooldown variables for bumpers
static unsigned long lastBumpBottomLeftActivation = 0;
static unsigned long lastBumpBottomRightActivation = 0;
static unsigned long lastBumpTopActivation = 0;
static unsigned long lastHillActivation = 0;
static unsigned long lastRmpExtActivation = 0;



void check_bumpers_left(unsigned long now){
    if (triggerDetectedBumpBottomLeft && !bumpBottomLeftActive) {
        if (now - lastBumpBottomLeftActivation <= 500) {
            triggerDetectedBumpBottomLeft = false; // Reset the trigger to avoid repeated activation
        }
        else{
        digitalWrite(SEL_BUMP_BOTTOM_LEFT, HIGH);
        Serial.println("Button press BUMP BOTTOM LEFT !!! .");
        leds_set_pixel(STRIP_PACK_CIBLE, 1, CRGB(255, 0, 0));
        FastLED.show();
        bumpBottomLeftTimer = now;
        bumpBottomLeftActive = true;
        triggerDetectedBumpBottomLeft = false;
        lastBumpBottomLeftActivation = now;
        }
    }
    if (bumpBottomLeftActive && now - bumpBottomLeftTimer >= 200) {
        digitalWrite(SEL_BUMP_BOTTOM_LEFT, LOW);
        leds_set_pixel(STRIP_PACK_CIBLE, 1, CRGB(0, 0, 0));
        FastLED.show();
        bumpBottomLeftActive = false;
        triggerDetectedBumpBottomLeft = false;
    }
}

void check_bumpers_right(unsigned long now){
    if (triggerDetectedBumpBottomRight && !bumpBottomRightActive ) {
        if (now - lastBumpBottomRightActivation <= 500) {
            triggerDetectedBumpBottomRight = false; // Reset the trigger to avoid repeated activation
        }
        else{
            digitalWrite(SEL_BUMP_BOTTOM_RIGHT, HIGH);
            Serial.println("Button press BUMP BOTTOM RIGHT !!! .");
            leds_set_pixel(STRIP_PACK_CIBLE, 2, CRGB(255, 0, 0));
            FastLED.show();
            bumpBottomRightTimer = now;
            bumpBottomRightActive = true;
            triggerDetectedBumpBottomRight = false;
            lastBumpBottomRightActivation = now;
        }
    }
    if (bumpBottomRightActive && now - bumpBottomRightTimer >= 200) {
        digitalWrite(SEL_BUMP_BOTTOM_RIGHT, LOW);
        leds_set_pixel(STRIP_PACK_CIBLE, 2, CRGB(0, 0, 0));
        FastLED.show();
        bumpBottomRightActive = false;
        triggerDetectedBumpBottomRight = false;
    }
}

void check_bumpers_top(unsigned long now){
    if (triggerDetectedBumpTop && !bumpTopActive) {
        if (now - lastBumpTopActivation <= 500) {
            triggerDetectedBumpTop = false; // Reset the trigger to avoid repeated activation
        }
        else{
            digitalWrite(SEL_BUMP_TOP, HIGH);
            Serial.println("Button press BUMP TOP !!! .");
            leds_set_pixel(STRIP_PACK_CIBLE, 0, CRGB(255, 0, 0));
            FastLED.show();
            bumpTopTimer = now;
            bumpTopActive = true;
            triggerDetectedBumpTop = false;
            lastBumpTopActivation = now;
        }
    }
    if (bumpTopActive && now - bumpTopTimer >= 200) {
        digitalWrite(SEL_BUMP_TOP, LOW);
        leds_set_pixel(STRIP_PACK_CIBLE, 0, CRGB(0, 0, 0));
        FastLED.show();
        bumpTopActive = false;
        triggerDetectedBumpTop = false;
    }
}

void check_hill(unsigned long now) {

    // --- ÉTAPE 1 : DÉTECTION ET DÉMARRAGE DU COMPTE À REBOURS ---
    if (triggerDetectedRmpHill) {
        // On vérifie le cooldown de 2 secondes par rapport à la dernière détection
        // Et on vérifie qu'on n'est pas déjà en train d'attendre (pour ne pas reset le timer)
        if ((now - lastHillActivation > 2000) && !isWaitingForHill && !triggerActiveHill) {
            
            Serial.println("Trigger Hill détecté ! Attente de 3 secondes...");
            
            // On entre en mode "Attente"
            isWaitingForHill = true;
            timerDelayHillStart = now; // On note l'heure de départ du délai
            
            // On met à jour le cooldown tout de suite pour éviter les doubles déclenchements
            lastHillActivation = now; 
        }
        
        // On reset toujours le flag de l'interruption hardware
        triggerDetectedRmpHill = false; 
    }

    // --- ÉTAPE 2 : VÉRIFICATION DU DÉLAI DE 3 SECONDES ---
    if (isWaitingForHill) {
        // Si 3000ms (3s) se sont écoulées depuis la détection
        if (now - timerDelayHillStart >= 2000) {
            // Le délai est fini, on passe à l'ACTION
            isWaitingForHill = false; // On arrête d'attendre
            
            digitalWrite(SEL_HILL, HIGH);
            Serial.println("ACTION HILL (après 3s de retard) !!!");
            leds_set_pixel(STRIP_PACK_CIBLE, 3, CRGB(255, 0, 0));
            FastLED.show();

            // On lance le timer pour la durée d'activation (50ms)
            triggerTimerHill = now;
            triggerActiveHill = true;
        }
    }

    // --- ÉTAPE 3 : FIN DE L'IMPULSION (100ms) ---
    if (triggerActiveHill && (now - triggerTimerHill >= 500)) {
        digitalWrite(SEL_HILL, LOW);
        leds_set_pixel(STRIP_PACK_CIBLE, 3, CRGB(0, 0, 0));
        FastLED.show();
        triggerActiveHill = false;
    }
}

void check_rmp_ext(unsigned long now) {
    if (triggerDetectedRmpExt) {
        if (now - lastRmpExtActivation <= 5000) {
            triggerDetectedRmpExt = false; // Reset pour éviter double déclenchement
        }
        else {
            // --- ACTION (Juste le Print) ---
            Serial.println("Detection RMP EXT !!!"); 
            triggerDetectedRmpExt = false;
            lastRmpExtActivation = now;
        }
    }
}

// Print which target was hit and reset the corresponding flags
void check_targets(unsigned long now) {
    if (triggerDetectedCiblePack1) {
        Serial.println("Detection PACK CIBLE 1 !!!");
        triggerDetectedCiblePack1 = false;
    }
    if (triggerDetectedCiblePack2) {
        Serial.println("Detection PACK CIBLE 2 !!!");
        triggerDetectedCiblePack2 = false;
    }
    if (triggerDetectedCiblePack3) {
        Serial.println("Detection PACK CIBLE 3 !!!");
        triggerDetectedCiblePack3 = false;
    }
    if (triggerDetectedCibleCowboyTop) {
        Serial.println("Detection CIBLE COWBOY TOP !!!");
        triggerDetectedCibleCowboyTop = false;
    }
}

void loop() {
    unsigned long now = millis();

    // Handle main trigger (SEL_HILL)
    check_hill(now);
    // Check bumpers
    check_bumpers_left(now);
    check_bumpers_right(now);
    check_bumpers_top(now);
    check_rmp_ext(now);
    // Check and print target detections
    check_targets(now);
    // Update LEDs (animations + blink handling)
    leds_update();
}