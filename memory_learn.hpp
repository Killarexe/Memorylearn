#include "esp32-hal-timer.h"
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
#include <Adafruit_NeoPixel.h>
#include <rgb_lcd.h>

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

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

#define BUTTON_LEFT BUTTON_1
#define BUTTON_RIGHT BUTTON_2
#define BUTTON_OK BUTTON_3
#define BUTTON_NO BUTTON_4

static const unsigned int BUTTONS_PINS[8] = {3, 4, 5, 6, 7, 8, 9, 10};

//TODO: Find names for the games...
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
  uint16_t max_reaction_time;
  BLECharacteristic* best_score; //TODO: Later
} SimonGame;

typedef struct LEDReact {
  uint8_t level;
  uint16_t reaction_time;
  uint16_t max_reaction_time;
  uint8_t state;
  BLECharacteristic* best_score;
  BLECharacteristic* best_reaction_time;
} LEDReact;

typedef struct MemoryLED {
  uint8_t level;
  uint8_t state;
  uint16_t max_reaction_time;
  BLECharacteristic* best_score; //TODO: Later
} MemoryLED;

typedef struct ColorLED {
  uint8_t level;
  uint8_t state;
  uint16_t max_reaction_time;
  BLECharacteristic* best_score; //TODO: Later
} ColorLED;

typedef struct MemoryLearn {
  //General variables
  uint8_t buttons;
  MemoryLearnState state;

  //Jeux/Menus
  SelectGame select_game;
  SimonGame simon_game;
  LEDReact led_react;
  MemoryLED memory_led;
  ColorLED color_led;

  //Hardware Managers
  Adafruit_NeoPixel* leds;
  rgb_lcd lcd;
  hw_timer_t* buzzer_timer;
  BuzzerDriver buzzer;

  //Bluetooth variables
  BLEServer* ble_server;
  BLEService* ble_service;
  BLEAdvertising ble_advertising;
  BLECharacteristic* best_score;  //Pour tester
} MemoryLearn;

void memory_learn_error(MemoryLearn* memory_learn, const char* error_message);

void memory_learn_set_state(MemoryLearn* memory_learn, MemoryLearnState state);

void memory_learn_init(MemoryLearn* memory_learn);

void memory_learn_update(MemoryLearn* memory_learn);

#endif
