#include "about.hpp"

void about_init(MemoryLearn* memory_learn) {
  memory_learn->lcd.clear();
  memory_learn->lcd.setCursor(0, 0);
  memory_learn->lcd.print("-=-=-About-=-=-=");
  memory_learn->lcd.setCursor(0, 1);
  memory_learn->lcd.print("MemoryLearn v1.0");
}

void about_update(MemoryLearn* memory_learn) {
  if (memory_learn->buttons & BUTTON_2) {
    memory_learn_set_state(memory_learn, MemoryLearnState::SELECT_GAME);
  }
}