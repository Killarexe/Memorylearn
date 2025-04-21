#include "esp32-hal-gpio.h"
#include "esp_timer.h"
#include "Esp.h"
#include "esp32-hal-timer.h"
#include "memory_learn.hpp"
#include "musics.hpp"
#include "lcd_chars.hpp"
#include "select_game.hpp"
//#include "bluetooth_manager.hpp"
#include "simon_game.hpp"
#include "about.hpp"
#include <rgb_lcd.h>

// Fonction appelée tout les 0.1ms 
void IRAM_ATTR on_buzzer_timer_interrupt(void* buzzer_driver) {
  update_buzzer_driver((BuzzerDriver*)buzzer_driver);
}

// Vérifie l'état des boutons et met le tout dans un octet
void update_buttons(MemoryLearn* memory_learn) {
  uint8_t buttons = 0;
  uint8_t previous_buttons = memory_learn->buttons;

  for (uint8_t i = 0; i < 8; i++) {
    buttons |= (digitalRead(BUTTONS_PINS[i]) & 1) << i;
  }
  buttons = ~buttons;

  memory_learn->just_pressed_buttons = (buttons ^ previous_buttons) & buttons;
  memory_learn->buttons = buttons;
  if (previous_buttons != buttons) {
    Serial.print("Buttons: ");
    Serial.print(buttons, BIN);
    Serial.print(", Just pressed buttons: ");
    Serial.println(memory_learn->just_pressed_buttons, BIN);
  }
}

void memory_learn_boot(MemoryLearn* memory_learn) {
  // Initialize le serial pour debug.
  Serial.begin(115200);
  Serial.println("Initialzing MemoryLearn!");

  // Initialize l'écran LCD avec les charatères customs
  Serial.println("Initialzing LCD Screen...");
  memory_learn->lcd.begin(16, 2);
  memory_learn->lcd.createChar(1, TOP_LEFT_ARROW);
  memory_learn->lcd.createChar(2, BOTTOM_LEFT_ARROW);
  memory_learn->lcd.createChar(3, TOP_RIGHT_ARROW);
  memory_learn->lcd.createChar(4, BOTTOM_RIGHT_ARROW);
  memory_learn->lcd.createChar(5, ERROR_ICON);
  memory_learn->lcd.clear();

  // Initialize les LED RGB
  Serial.println("Initialzing LEDs...");
  memory_learn->leds = new Adafruit_NeoPixel(8, LEDS_PIN, NEO_GRB + NEO_KHZ800);
  if (memory_learn->leds == nullptr) {
    Serial.println("Error: Failed to initialze LEDs");
  } else {
    memory_learn->leds->clear();
    memory_learn->leds->show();
  }
  memory_learn->lcd.clear();

  // Met les pins des boutons en INPUT
  Serial.println("Initialzing buttons...");
  for (uint8_t i = 0; i < 8; i++) {
    pinMode(BUTTONS_PINS[i], INPUT_PULLUP);
  }

  // Initialize le driver du buzzer
  Serial.println("Initialzing buzzer driver...");
  init_buzzer_driver(&memory_learn->buzzer, BUZZER_PIN);

  // Crée une interuption tout les 0.1ms pour le driver du buzzer.
  Serial.println("Creating buzzer driver timer interrupt...");
  memory_learn->buzzer_timer = timerBegin(100000);
  if (!memory_learn->buzzer_timer) {
    Serial.println("Failed to create timer for the buzzer driver...");
  } else {
    timerAttachInterruptArg(memory_learn->buzzer_timer, &on_buzzer_timer_interrupt, &memory_learn->buzzer);
    timerAlarm(memory_learn->buzzer_timer, 1, true, 0);
  }

  // Initialize la partie Bluetooth
  //Serial.println("Starting Bluetooth device...");
  //bluetooth_start(memory_learn);

  memory_learn->previous_time = esp_timer_get_time();
  memory_learn->just_pressed_buttons = 0;
  play_buzzer_driver(&memory_learn->buzzer, INTRO);
  memory_learn->lcd.clear();
  memory_learn->lcd.setCursor(0, 0);
  memory_learn->lcd.print("   Memo");
  delay(500);
  memory_learn->lcd.print("ry");
  delay(500);
  memory_learn->lcd.print("Learn");
  delay(500);
  memory_learn->lcd.setCursor(0, 1);
  memory_learn->lcd.print("Press any button");
  while (!memory_learn->just_pressed_buttons) {
    update_buttons(memory_learn);
  }
  // Initialize le menu de selection des jeux
  memory_learn_set_state(memory_learn, MemoryLearnState::SELECT_GAME);
}

// Met le MemoryLearn en état d'erreur, et attend qu'un bouton est appuyé pour redémarrer. 
void memory_learn_error(MemoryLearn* memory_learn, const char* error_message) {
  memory_learn->lcd.clear();
  memory_learn->lcd.setCursor(0, 0);
  memory_learn->lcd.print("\5-=-/Error.\\-=-\5");
  memory_learn->lcd.setCursor(0, 1);
  memory_learn->lcd.print(error_message);

  // Attend qu'un bouton soit appuyé.
  for (uint8_t i = 0; i < 8; i++) {
    memory_learn->leds->setPixelColor(i, 4, 0, 0);
  }
  memory_learn->leds->show();

  update_buttons(memory_learn);
  while (!memory_learn->just_pressed_buttons) {
    update_buttons(memory_learn);
  }
  ESP.restart();
}

// Change l'état du MemoryLearn. Et appele la fonction d'initialisation pour l'état.
void memory_learn_set_state(MemoryLearn* memory_learn, MemoryLearnState state) {
  memory_learn->state = state;
  memory_learn->lcd.clear();
  Serial.print("Setting MemoryLearn state to: ");
  Serial.println((unsigned char)state);
  switch (state) {
    case MemoryLearnState::SELECT_GAME:
      select_game_init(memory_learn);
      break;
    case MemoryLearnState::SIMON:
      simon_game_init(memory_learn);
      break;
    case MemoryLearnState::ABOUT:
      about_init(memory_learn);
      break;
    default:
      // Erreur si l'état demandé n'existe pas ou pas référé ici.
      memory_learn_error(memory_learn, "Unexpected State");
      break;
  }
}

// Met juste en état de démarage.
void memory_learn_init(MemoryLearn *memory_learn) {
  memory_learn->state = MemoryLearnState::BOOT;
  memory_learn_boot(memory_learn);
}

// Fonction loop du MemoryLearn
void memory_learn_update(MemoryLearn *memory_learn) {
  // Calcue le temps passée avant la dernière appel.
  unsigned long current_time = millis();
  unsigned long delta_time = current_time - memory_learn->previous_time;
  memory_learn->previous_time = current_time;
  update_buttons(memory_learn); // Met à jour les boutons
  // Met à jour l'état actuelle du MemoryLearn
  switch (memory_learn->state) {
    case MemoryLearnState::SELECT_GAME:
      select_game_update(memory_learn);
      break;
    case MemoryLearnState::SIMON:
      simon_game_update(memory_learn, delta_time);
      break;
    case MemoryLearnState::ABOUT:
      about_update(memory_learn, delta_time);
      break;
    default:
      // Erreur si l'état demandé n'existe pas ou pas référé ici.
      memory_learn_error(memory_learn, "Unexpected State");
      break;
  }
}
