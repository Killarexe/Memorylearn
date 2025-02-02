#ifndef MEMORY_LEARN_H
#define MEMORY_LEARN_H

/*
 * Memorylearn PIN Layout:
 * A0-A5, 2, 3 => Buttons
 * 4, 5 => LCD Screen
 * 6, 7 => Bluetooth device
 * 8 => Buzzer PIN
 * 9 => Adafruit Neopixel RGB LED
 */

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
