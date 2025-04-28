#ifndef COLOR_LED_HPP
#define COLOR_LED_HPP

#define COLOR_LED_STATE_MENU 0
#define COLOR_LED_STATE_SHOW 1
#define COLOR_LED_STATE_PLAY 2
#define COLOR_LED_STATE_GAMEOVER 3

#include "memory_learn.hpp"

void color_led_init(MemoryLearn* memory_learn);

void color_led_update(MemoryLearn* memory_learn, unsigned long delta_time);

#endif
