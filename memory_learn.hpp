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
#include "lcd_driver.hpp"

#define BUZZER_PIN 8
#define LCD_SDA_PIN 4
#define LCD_SCL_PIN 5
#define LEDS_PIN 9

static const unsigned int BUTTONS_PINS[8] = {0, 0, 0, 0, 0, 0, 0, 0};

typedef enum MemoryLearnState {
  BOOT,
  SELECT_GAME,
} MemoryLearnState;

typedef struct MemoryLearn {
  MemoryLearnState state;
  BuzzerDriver buzzer;
  LCD lcd;
  uint8_t buttons;
} MemoryLearn;

void init_memory_learn(MemoryLearn* memory_learn);

void update_memory_learn(MemoryLearn* memory_learn);

#endif
