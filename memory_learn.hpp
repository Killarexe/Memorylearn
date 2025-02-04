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

#define LCD_PIN 4
#define LEDS_PIN 9

#include "buzzer_driver.hpp"

typedef enum MemoryLearnState {
  BOOT,
  SELECT_GAME,
} MemoryLearnState;

typedef struct MemoryLearn {
  MemoryLearnState state;
  BuzzerDriver buzzer;
} MemoryLearn;

#endif
