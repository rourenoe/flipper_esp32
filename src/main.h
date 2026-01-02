#ifndef MAIN_H
#define MAIN_H
#include <FastLED.h>
// Définir des constantes ou des variables communes
#define PC_START 48

//Montagne
#define SEL_HILL 21 
#define RMP_HILL 38

//Bump
#define BUMP_BOTTOM_LEFT 47
#define SEL_BUMP_BOTTOM_LEFT 5

#define BUMP_BOTTOM_RIGHT 17
#define SEL_BUMP_BOTTOM_RIGHT 6

#define BUMP_TOP 35
#define SEL_BUMP_TOP 11


//Motor 
#define MOTOR_LAUNCH_BALL 12
#define MOTOR_MULTIBALL 41


#define DETECTION_RMP_EXT 39

//Cibles
#define DETECTION_PACK_CIBLE_1 1
#define DETECTION_PACK_CIBLE_2 2
#define DETECTION_PACK_CIBLE_3 42

#define DETECTION_CIBLE_COWBOY_TOP 18
//Leds
#define NUM_LEDS 53
#define LED_PACK_CIBLE 4
#define LED_DRAGON 13
#define LED_SALOON 19



extern CRGB leds[NUM_LEDS];  // Déclaration externe de leds pour que Fire.cpp puisse y accéder

#endif // MAIN_H
