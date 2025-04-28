/* memory_learn.hpp
 *
 * Defintion de toutes les fonctions, structures de base et des jeux du MemoryLearn.
 */

#ifndef MEMORY_LEARN_HPP
#define MEMORY_LEARN_HPP

#include "buzzer_driver.hpp"
#include <Adafruit_NeoPixel.h>
#include <rgb_lcd.h>

//#include <BLEDevice.h>
//#include <BLEUtils.h>
//#include <BLEServer.h>

#include "esp32-hal-timer.h"

#define BUTTON_1 1
#define BUTTON_2 2
#define BUTTON_3 4
#define BUTTON_4 8
#define BUTTON_5 16
#define BUTTON_6 32
#define BUTTON_7 64
#define BUTTON_8 128

#define BUTTON_LEFT BUTTON_2
#define BUTTON_RIGHT BUTTON_5
#define BUTTON_OK BUTTON_1
#define BUTTON_NO BUTTON_6

// Pins defintion
#define BUZZER_PIN 32
#define LEDS_PIN 13

#define BUTTONS_UPDATE_RATE 100

static const unsigned int BUTTONS_PINS[8] = {2, 14, 16, 4, 27, 26, 5, 25};

typedef enum MemoryLearnState {
  BOOT,
  SELECT_GAME,
  SIMON,
  LED_REACT,
  MEMORY_LED,
  COLOR_LED,
  ABOUT
} MemoryLearnState;

typedef struct SelectGame {
  uint8_t cursor_index;
} SelectGame;

typedef struct SimonGame {
  uint8_t level;
  uint8_t state;
  uint8_t* buttons;
  uint8_t button_index;
  unsigned long reaction_time;
} SimonGame;

typedef struct LEDReact {
  uint8_t level;
  unsigned long accumulated_time;
  uint8_t state;
  uint8_t correct_button;
} LEDReact;

typedef struct MemoryLED {
  uint8_t level;
  uint8_t state;
  uint8_t correct_button;
  unsigned long accumulated_time;
} MemoryLED;

typedef struct ColorLED {
  uint8_t level;
  uint8_t state;
  uint8_t correct_button;
  unsigned long accumulated_time;
} ColorLED;

typedef struct AboutMenu {
  int64_t passed_time;
  uint8_t credits;
} AboutMenu;

typedef struct MemoryLearn {
  //General variables
  uint8_t buttons;
  uint8_t just_pressed_buttons;
  MemoryLearnState state;
  unsigned long previous_time;
  unsigned long button_time;

  //Jeux/Menus
  SelectGame select_game;
  SimonGame simon_game;
  LEDReact led_react;
  ColorLED color_led;
  AboutMenu about_menu;

  //Hardware Managers/Drivers
  Adafruit_NeoPixel* leds;
  rgb_lcd lcd;
  hw_timer_t* buzzer_timer;
  BuzzerDriver buzzer;
} MemoryLearn;

/**
 * Met le MemoryLearn en mode erreur. Et affiche l'erreur sur l'écran LCD.
 * @param MemoryLearn* memory_learn
 * @param const char* error_message Le message d'erreur
 */
void memory_learn_error(MemoryLearn* memory_learn, const char* error_message);

/**
 * Change l'êtat du MemoryLearn et initialize l'état.
 * @param MemoryLearn* memory_learn
 * @param MemoryLearnState state Le nouvel état du MemoryLearn
 */
void memory_learn_set_state(MemoryLearn* memory_learn, MemoryLearnState state);

/**
 * Initialize le MemoryLearn et prepare tout les composants.
 * @param MemoryLearn* memory_learn
 */
void memory_learn_init(MemoryLearn* memory_learn);

/**
 * Met à jour le MemoryLearn dépendents dans sont état. 
 */
void memory_learn_update(MemoryLearn* memory_learn);

#endif
