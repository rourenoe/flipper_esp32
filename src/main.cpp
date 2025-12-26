#include "main.h"
// Paramètres de configuration des LEDs
CRGB leds[NUM_LEDS];  // Déclaration de leds ici

volatile bool triggerDetectedBumpBottomLeft = false;
volatile bool triggerDetectedBumpBottomRight = false;
volatile bool triggerDetectedBumpTop = false;
volatile bool triggerDetectedRmpHill = false;
volatile bool triggerDetectedRmpExt = false;

unsigned long timerDelayHillStart = 0; // Pour compter les 3 secondes
bool isWaitingForHill = false;

void IRAM_ATTR onTriggerRmpExt() {
    triggerDetectedRmpExt = true;
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


  FastLED.addLeds<WS2812B, LED_PACK_CIBLE, GRB>(leds, NUM_LEDS);
  //FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(100); // Changé de 0 à 100 pour voir les LEDs
  //FastLED.setMaxPowerInVoltsAndMilliamps(5, 14000);

  Serial.begin(115200);
  for (int i = 0; i < 40; i++) {
            leds[i] = CRGB::Red;
        
    }
    for (int i = 40; i < 53; i++) {
            leds[i] = CRGB::Blue;
        
    }
    FastLED.clear();
    FastLED.show();
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


void update_led_pc() {
    static int redBrightness = 0;
    static int blueBrightness = 255;
    static int redDirection = 3;
    static int blueDirection = -1;
    static unsigned long lastUpdate = 0;
    unsigned long now = millis();
    if (now - lastUpdate >= 5) {
        // Update red LEDs (0-39)
        for (int i = 0; i < 40; i++) {
            leds[i] = CRGB(redBrightness, 0, 0);
        }
        // Update blue LEDs (40-52)
        for (int i = 40; i < 53; i++) {
            leds[i] = CRGB(0, 0, blueBrightness);
        }
        FastLED.show();
        // Update brightness values and directions
        redBrightness += redDirection;
        blueBrightness += blueDirection;
        // Constrain brightness values and change direction
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
        lastUpdate = now;
    }
}

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
        leds[1] = CRGB(255, 0, 0);
        FastLED.show();
        bumpBottomLeftTimer = now;
        bumpBottomLeftActive = true;
        triggerDetectedBumpBottomLeft = false;
        lastBumpBottomLeftActivation = now;
        }
    }
    if (bumpBottomLeftActive && now - bumpBottomLeftTimer >= 200) {
        digitalWrite(SEL_BUMP_BOTTOM_LEFT, LOW);
        leds[1] = CRGB(0, 0, 0);
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
            leds[2] = CRGB(255, 0, 0);
            FastLED.show();
            bumpBottomRightTimer = now;
            bumpBottomRightActive = true;
            triggerDetectedBumpBottomRight = false;
            lastBumpBottomRightActivation = now;
        }
    }
    if (bumpBottomRightActive && now - bumpBottomRightTimer >= 200) {
        digitalWrite(SEL_BUMP_BOTTOM_RIGHT, LOW);
        leds[2] = CRGB(0, 0, 0);
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
            leds[0] = CRGB(255, 0, 0);
            FastLED.show();
            bumpTopTimer = now;
            bumpTopActive = true;
            triggerDetectedBumpTop = false;
            lastBumpTopActivation = now;
        }
    }
    if (bumpTopActive && now - bumpTopTimer >= 200) {
        digitalWrite(SEL_BUMP_TOP, LOW);
        leds[0] = CRGB(0, 0, 0);
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
            leds[3] = CRGB(255, 0, 0);
            FastLED.show();

            // On lance le timer pour la durée d'activation (50ms)
            triggerTimerHill = now;
            triggerActiveHill = true;
        }
    }

    // --- ÉTAPE 3 : FIN DE L'IMPULSION (100ms) ---
    if (triggerActiveHill && (now - triggerTimerHill >= 500)) {
        digitalWrite(SEL_HILL, LOW);
        leds[3] = CRGB(0, 0, 0);
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

void loop() {
    unsigned long now = millis();

    // Handle main trigger (SEL_HILL)
    check_hill(now);
    // Check bumpers
    check_bumpers_left(now);
    check_bumpers_right(now);
    check_bumpers_top(now);
    check_rmp_ext(now);
    update_led_pc();
}