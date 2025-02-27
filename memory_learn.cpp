#include "memory_learn.hpp"
#include "lcd_chars.hpp"
#include "pico/stdlib.h"
#include "hardware/timer.h"

bool buzzer_update_callback(struct repeating_timer* timer) {
  update_buzzer_driver((BuzzerDriver*)timer->user_data);
  return true;
}

void memory_learn_boot(MemoryLearn* memory_learn) {
  lcd_init(&memory_learn->lcd, 0x27, PICO_DEFAULT_I2C_SDA_PIN, PICO_DEFAULT_I2C_SCL_PIN, i2c0);
  lcd_create_char(&memory_learn->lcd, 0, TOP_LEFT_ARROW);
  lcd_create_char(&memory_learn->lcd, 1, BOTTOM_LEFT_ARROW);
  lcd_create_char(&memory_learn->lcd, 2, TOP_RIGHT_ARROW);
  lcd_create_char(&memory_learn->lcd, 3, BOTTOM_RIGHT_ARROW);
  lcd_print(&memory_learn->lcd, "Hello, world!");
  if (memory_learn->leds == 0) {
    memory_learn->leds = new Adafruit_NeoPixel(8, LEDS_PIN, NEO_GRB + NEO_KHZ800);
  }
  memory_learn->leds->clear();

  for (uint8_t i = 0; i < 8; i++) {
    gpio_init(BUTTONS_PINS[i]);
    gpio_set_dir(BUTTONS_PINS[i], GPIO_IN);
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
    memory_learn->buttons |= gpio_get(BUTTONS_PINS[i]);
  }
}

void memory_learn_update(MemoryLearn *memory_learn) {
  update_buttons(memory_learn);
}
