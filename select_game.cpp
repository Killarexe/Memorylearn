#include "select_game.hpp"
#include "memory_learn.hpp"
#include <string.h>

// Nom des jeux
const char NAMES[5][17] = {
  "\1    Simon     \3",
  "\1  LED React   \3",
  "\1   MemoLED    \3",
  "\1  Color LED   \3",
  "\1    About     \3"
};

// Etats possibles
const MemoryLearnState STATES[5] = {
  MemoryLearnState::SIMON,
  MemoryLearnState::COLOR_LED,
  MemoryLearnState::LED_REACT,
  MemoryLearnState::MEMORY_LED,
  MemoryLearnState::ABOUT
};

// Affiche le jeu séléctionée sur l'écran LCD
void select_game_update_select(MemoryLearn* memory_learn) {
  tone_buzzer_driver(&memory_learn->buzzer, 440, 125);
  memory_learn->lcd.clear();
  memory_learn->lcd.setCursor(0, 0);
  memory_learn->lcd.print(NAMES[memory_learn->select_game.cursor_index]);
  memory_learn->lcd.setCursor(0, 1);
  memory_learn->lcd.print("\2 Press button \4");
  Serial.print("Selected game n°");
  Serial.println(memory_learn->select_game.cursor_index);
}

// Initialize le menu
void select_game_init(MemoryLearn* memory_learn) {
  memory_learn->select_game.cursor_index = 0;
  select_game_update_select(memory_learn);
}

// Met a jour le menu en verifient les boutons droite et gauche pour la sélécton, et le bouton OK pour jouer.
void select_game_update(MemoryLearn* memory_learn) {
  if (memory_learn->just_pressed_buttons & BUTTON_LEFT) {
    memory_learn->select_game.cursor_index++;
    memory_learn->select_game.cursor_index %= 5;
    select_game_update_select(memory_learn);
  } else if (memory_learn->just_pressed_buttons & BUTTON_RIGHT) {
    if (memory_learn->select_game.cursor_index == 0) {
      memory_learn->select_game.cursor_index = 4;
    } else {
      memory_learn->select_game.cursor_index--;
    }
    select_game_update_select(memory_learn);
  } else if (memory_learn->just_pressed_buttons & BUTTON_OK) {
    memory_learn_set_state(memory_learn, STATES[memory_learn->select_game.cursor_index]);
  }
}
