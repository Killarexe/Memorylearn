#include "select_game.hpp"
#include "led.hpp"

void select_game_init(MemoryLearn* memory_learn) {
  lcd_clear(&memory_learn->lcd);
  lcd_print(&memory_learn->lcd, "Select Game");
  play_buzzer_driver(&memory_learn->buzzer, LED_MUSIC);
}

void select_game_update(MemoryLearn* memory_learn) {
  if (memory_learn->buttons & BUTTON_2) {
    memory_learn->select_game.cursor_index++;
  } else if (memory_learn->buttons & BUTTON_1) {
    memory_learn->select_game.cursor_index--;
  } else if (memory_learn->buttons & BUTTON_3) {
    switch (memory_learn->select_game.cursor_index) {
      //TODO: Make this work
    }
  }
}