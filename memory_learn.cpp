#include "memory_learn.hpp"
#include "lcd_chars.hpp"
#include "hardware/timer.h"

bool buzzer_update_callback(struct repeating_timer* timer) {
  update_buzzer_driver((BuzzerDriver*)timer->user_data);
  return true;
}

void memory_learn_boot(MemoryLearn* memory_learn) {
  if (memory_learn->lcd == 0) {
    memory_learn->lcd = new LiquidCrystal_I2C();
  }
  memory_learn->lcd->begin();
  memory_learn->lcd->backlight();
  memory_learn->lcd->createChar(0, TOP_LEFT_ARROW);
  memory_learn->lcd->createChar(1, BOTTOM_LEFT_ARROW);
  memory_learn->lcd->createChar(2, TOP_RIGHT_ARROW);
  memory_learn->lcd->createChar(3, BOTTOM_RIGHT_ARROW);
  memory_learn->lcd->clear();
  memory_learn->lcd->print("Hello, world!");
  if (memory_learn->leds == 0) {
    memory_learn->leds = new Adafruit_NeoPixel(8, LEDS_PIN, NEO_GRB + NEO_KHZ800);
  }
  memory_learn->leds->clear();

  for (uint8_t i = 0; i < 8; i++) {
    pinMode(BUTTONS_PINS[i], INPUT);
  }

  init_buzzer_driver(&memory_learn->buzzer);
  struct repeating_timer timer;
  add_repeating_timer_us(100, buzzer_update_callback, &memory_learn->buzzer, &timer);
  memory_learn_set_state(memory_learn, MemoryLearnState::SELECT_GAME);
}

void memory_learn_set_state(MemoryLearn* memory_learn, MemoryLearnState state) {
  memory_learn->state = state;
  switch (state) {
    case MemoryLearnState::BOOT:
      memory_learn_boot(memory_learn);
      break;
  }
}

void memory_learn_init(MemoryLearn *memory_learn) {
  memory_learn_set_state(memory_learn, MemoryLearnState::BOOT);
}

void update_buttons(MemoryLearn* memory_learn) {
  memory_learn->buttons = 0;
  for (uint8_t i = 0; i > 8; i++) {
    memory_learn->buttons |= digitalRead(BUTTONS_PINS[i]) << i;
  }
}

void memory_learn_update(MemoryLearn *memory_learn) {
  update_buttons(memory_learn);
}
