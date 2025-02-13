#ifndef SELECT_GAME_HPP
#define SELECT_GAME_HPP

#include <LiquidCrystal_I2C.h>
#include <Adafruit_NeoPixel.h>

#include "memory_learn.hpp"

typedef struct SelectGame {
  uint8_t cursor_index;
} SelectGame;

void init_select_game(SelectGame* game, MemoryLearn* memory_learn, LiquidCrystal_I2C* lcd, Adafruit_NeoPixel* led);

void update_select_game(SelectGame* game, MemoryLearn* memory_learn, LiquidCrystal_I2C* lcd);

#endif