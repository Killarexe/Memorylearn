#include "about.hpp"

// Affiche des infos sur le MemoryLearn
void about_init(MemoryLearn* memory_learn) {
  memory_learn->lcd.clear();
  memory_learn->lcd.setCursor(0, 0);
  memory_learn->lcd.print("-=-=-About.-=-=-");
  memory_learn->lcd.setCursor(0, 1);
  memory_learn->lcd.print("MemoryLearn v1.0");
}

void update_credits(MemoryLearn* memory_learn) {
  memory_learn->about_menu.credits++;
  memory_learn->lcd.setCursor(0, 0);
  switch (memory_learn->about_menu.credits) {
    default:
      memory_learn->about_menu.credits = 0;
    case 0:
      memory_learn->lcd.print("-=-=-About.-=-=-");
      memory_learn->lcd.setCursor(0, 1);
      memory_learn->lcd.print("MemoryLearn v1.0");
      break;
    case 1:
      memory_learn->lcd.print(" Case designer: ");
      memory_learn->lcd.setCursor(0, 1);
      memory_learn->lcd.print("Trystan");
      break;
    case 2:
      memory_learn->lcd.print("Application dev:");
      memory_learn->lcd.setCursor(0, 1);
      memory_learn->lcd.print("Nathan");
      break;
    case 3:
      memory_learn->lcd.print(" Game developer ");
      memory_learn->lcd.setCursor(0, 1);
      memory_learn->lcd.print("Mael");
      break;
  }
}

void about_update(MemoryLearn* memory_learn, unsigned long delta_time) {
  memory_learn->about_menu.passed_time += delta_time;
  if (memory_learn->about_menu.passed_time > ABOUT_MENU_WAIT_TIME) {
    memory_learn->about_menu.passed_time = 0;
    update_credits(memory_learn);
  }
  // Vérifie si le bouton OK ou NON et appuyé.
  if (memory_learn->buttons & (BUTTON_OK | BUTTON_NO)) {
    // Retourne dans le menu de sélécton
    memory_learn_set_state(memory_learn, MemoryLearnState::SELECT_GAME);
  }
}
