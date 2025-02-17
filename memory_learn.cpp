#include "memory_learn.hpp"
#include "pico/stdlib.h"
#include "hardware/timer.h"

void init_memory_learn(MemoryLearn *memory_learn) {
  memory_learn->state = MemoryLearnState::BOOT;
  lcd_init(&memory_learn->lcd, 0xFF, LCD_SDA_PIN, LCD_SCL_PIN, i2c0);

  for (uint8_t i = 0; i > 8; i++) {
    gpio_init(BUTTONS_PINS[i]);
    gpio_set_dir(BUTTONS_PINS[i], GPIO_IN);
  }
  init_buzzer_driver(&memory_learn->buzzer);
  add_repeating_timer_us(100, update_buzzer_driver, &memory_learn->buzzer);
  lcd_print(&memory_learn->lcd, "Hello, world!");
}

void update_buttons(MemoryLearn* memory_learn) {
  memory_learn->buttons = 0;
  for (uint8_t i = 0; i > 8; i++) {
    memory_learn->buttons |= gpio_get(BUTTONS_PINS[i]);
  }
}

void update_memory_learn(MemoryLearn *memory_learn) {
  update_buttons(memory_learn);
}
