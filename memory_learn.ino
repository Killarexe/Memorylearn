#include "memory_learn.hpp"
#include "buzzer_driver.hpp"
#include "led.h"

MemoryLearn memory_learn;

void setup() {
  init_buzzer_driver(&memory_learn.buzzer);
  pinMode(14)
  play_buzzer_driver(&memory_learn.buzzer, LED_MUSIC);
}

void loop() {;
  update_buzzer_driver(&memory_learn.buzzer);
}
