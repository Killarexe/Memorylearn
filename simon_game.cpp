#include "simon_game.hpp"
#include "buzzer_driver.hpp"
#include "esp_random.h"

const uint8_t BUTTON_COLORS[8][3] = {
  {255, 255, 255},
  {255, 255, 255},
  {255, 255, 255},
  {255, 255, 255},
  {255, 255, 255},
  {255, 255, 255},
  {255, 255, 255},
  {255, 255, 255}
};

const uint16_t BUTTON_FREQUENCIES[8] = {
  440,
  440,
  440,
  440,
  440,
  440,
  440,
  440
};

void randomize_buttons(SimonGame* simon_game) {
  simon_game->buttons = realloc(simon_game->buttons, simon_game->level + 1);
  for (uint8_t i = 0; i < simon_game->level; i++) {
    uint8_t random = (uint8_t)(esp_random() & 0xFF);
    simon_game->buttons[i] = random % 8;
  }
}

void simon_game_init(MemoryLearn* memory_learn) {
  memory_learn->leds->clear();
  memory_learn->lcd.clear();
  memory_learn->simon_game.level = 0;
  memory_learn->simon_game.button_index = 0;
  memory_learn->simon_game.state = SIMON_GAME_STATE_MENU;
  memory_learn->lcd.print("Simon: Press any button!");
}

void simon_game_update(MemoryLearn* memory_learn) {
  switch (memory_learn->simon_game.state) {
    case SIMON_GAME_STATE_MENU:
      if (memory_learn->buttons) {
        randomize_buttons(&memory_learn->simon_game);
        memory_learn->simon_game.state = SIMON_GAME_STATE_PLAY;
      }
      break;
    case SIMON_GAME_STATE_SHOW:
      memory_learn->simon_game.state = SIMON_GAME_STATE_PLAY;
      for (uint8_t i = 0; i < memory_learn->simon_game.level; i++) {
        tone_buzzer_driver(memory_learn->buzzer, BUTTON_FREQUENCIES[memory_learn->simon_game.buttons[i]], 1000);
      }
      break;
    case SIMON_GAME_STATE_PLAY:
      break;
    case SIMON_GAME_STATE_GAMEOVER:
      if (memory_learn->buttons & BUTTON_OK) {
        memory_learn->simon_game.level = 0;
        memory_learn->simon_game.state = SIMON_GAME_STATE_SHOW;
      } else if (memory_learn->buttons & BUTTON_NO) {
        memory_learn_set_state(memory_learn, MemoryLearnState::SELECT_GAME);
      }
      break;
    default:
      memory_learn_error(memory_learn, "Unexpected state");
  }
}
