#ifndef MEMORY_LEARN_HPP
#define MEMORY_LEARN_HPP

/*
 * Memorylearn PIN Layout:
 * A0-A5, 2, 3 => Buttons
 * 4, 5 => LCD Screen
 * 6, 7 => Bluetooth device
 * 8 => Buzzer PIN
 * 9 => Adafruit Neopixel RGB LED
 */

#include "buzzer_driver.hpp"
#include "adafruit_neopixel.hpp"
#include <LiquidCrystal_I2C.h>

#define BUZZER_PIN 12
#define LEDS_PIN 2

#define BUTTON_1 1
#define BUTTON_2 2
#define BUTTON_3 4
#define BUTTON_4 8
#define BUTTON_5 16
#define BUTTON_6 32
#define BUTTON_7 64
#define BUTTON_8 128

static const unsigned int BUTTONS_PINS[8] = {0, 0, 0, 0, 0, 0, 0, 0};

//TODO: Find names for the games...
typedef enum MemoryLearnState {
  BOOT,
  SELECT_GAME,
  SIMON,
  LED_REACT,
  MEMORY_LED,
  COLOR_LED,
  BLUETOOTH
} MemoryLearnState;

typedef struct SelectGame {
  uint8_t cursor_index;
} SelectGame;

typedef struct MemoryLearn {
  uint8_t buttons;
  MemoryLearnState state;

  SelectGame select_game;


  Adafruit_NeoPixel* leds;
  LiquidCrystal_I2C* lcd;
  BuzzerDriver buzzer;
  LCD lcd;
} MemoryLearn;

void memory_learn_set_state(MemoryLearn* memory_learn, MemoryLearnState state);

void memory_learn_init(MemoryLearn* memory_learn);

void memory_learn_update(MemoryLearn* memory_learn);

#endif
