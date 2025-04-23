#ifndef MEMORY_LED_HPP
#define MEMORY_LED_HPP

#define MEMORY_LED_STATE_MENU 0
#define MEMORY_LED_STATE_SHOW 1
#define MEMORY_LED_STATE_PLAY 2
#define MEMORY_LED_STATE_GAMEOVER 3

#include "memory_learn.hpp"

void memory_led_init(MemoryLearn* memory_learn);

void memory_led_update(MemoryLearn* memory_learn, unsigned long delta_time);

#endif
