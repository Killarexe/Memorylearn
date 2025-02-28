#include "select_game.hpp"
#include "led.hpp"
#include "memory_learn.hpp"
#include <string.h>

const char* NAMES[5] = {
  "\1Simon",
  "\1Template",
  "\1Template",
  "\1Template",
  "\1Bluetooth"
};

const MemoryLearnState STATES = {
  MemoryLearnState::SIMON,
  MemoryLearnState::COLOR_LED,
  MemoryLearnState::LED_REACT,
  MemoryLearnState::MEMORY_LED,
  MemoryLearnState::BLUETOOTH
};

void select_game_init(MemoryLearn* memory_learn) {
  memory_learn->select_game.cursor_index = 0;
  select_game_update_select(memory_learn);
  play_buzzer_driver(&memory_learn->buzzer, LED_MUSIC);
}

void select_game_update_select(MemoryLearn* memory_learn) {
  const char* string = "\0              \2\1 Press Button \3";
  const char* name = NAMES[memory_learn->select_game.cursor_index];
  uint8_t offset = name[0] - 1;
  for (uint8_t i = 0; i < strlen(name); i++) {
    string[i + offset] = name[i];
  }
  memory_learn->lcd->setCursor(0, 0);
  memory_learn->lcd->print(string);
}

void select_game_update(MemoryLearn* memory_learn) {
  if (memory_learn->buttons & BUTTON_2 && memory_learn->select_game.cursor_index < 5) {
    memory_learn->select_game.cursor_index++;
    select_game_update_select(memory_learn);
  } else if (memory_learn->buttons & BUTTON_1 && memory_learn->select_game.cursor_index > 0) {
    memory_learn->select_game.cursor_index--;
    select_game_update_select(memory_learn);
  } else if (memory_learn->buttons & BUTTON_3) {
    memory_learn_set_state(memory_learn, STATES[memory_learn->select_game.cursor_index]);
  }
}
