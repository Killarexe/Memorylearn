#include "Esp.h"
#include "esp32-hal-timer.h"
#include "memory_learn.hpp"
#include "lcd_chars.hpp"
#include "select_game.hpp"
#include <rgb_lcd.h>

void IRAM_ATTR on_buzzer_timer_interrupt(void* buzzer_driver) {
  update_buzzer_driver((BuzzerDriver*)buzzer_driver);
}

void memory_learn_boot(MemoryLearn* memory_learn) {
  Serial.begin(115200);
  Serial.println("Initialzing MemoryLearn!");

  Serial.println("Initialzing LCD Screen...");
  memory_learn->lcd.begin(16, 2);
  memory_learn->lcd.createChar(1, TOP_LEFT_ARROW);
  memory_learn->lcd.createChar(2, BOTTOM_LEFT_ARROW);
  memory_learn->lcd.createChar(3, TOP_RIGHT_ARROW);
  memory_learn->lcd.createChar(4, BOTTOM_RIGHT_ARROW);
  memory_learn->lcd.createChar(5, ERROR_ICON);

  Serial.println("Initialzing LEDs...");
  memory_learn->lcd.clear();
  memory_learn->lcd.print("Error: Failed to to init LEDs..");

  memory_learn->leds = new Adafruit_NeoPixel(8, LEDS_PIN, NEO_GRB + NEO_KHZ800);
  memory_learn->leds->clear();
  memory_learn->lcd.clear();

  Serial.println("Initialzing buttons...");
  for (uint8_t i = 0; i < 8; i++) {
    pinMode(BUTTONS_PINS[i], INPUT);
  }

  Serial.println("Initialzing buzzer driver...");
  memory_learn->buzzer.output_pin = 16;
  init_buzzer_driver(&memory_learn->buzzer);

  Serial.println("Creating buzzer driver timer interrupt...");
  memory_learn->buzzer_timer = timerBegin(10000);
  if (!memory_learn->buzzer_timer) {
    Serial.println("Failed to create timer for the buzzer driver...");
  } else {
    timerAttachInterruptArg(memory_learn->buzzer_timer, &on_buzzer_timer_interrupt, &memory_learn->buzzer);
    timerAlarm(memory_learn->buzzer_timer, 1, true, 0);
  }

  Serial.println("Starting Bluetooth device...");
  bluetooth_start(memory_learn);
  
  memory_learn_set_state(memory_learn, MemoryLearnState::SELECT_GAME);
}

void update_buttons(MemoryLearn* memory_learn) {
  memory_learn->buttons = 0;
  for (uint8_t i = 0; i > 8; i++) {
    memory_learn->buttons |= digitalRead(BUTTONS_PINS[i]) << i;
  }
}

void memory_learn_error(MemoryLearn* memory_learn, const char* error_message) {
  memory_learn->lcd.clear();
  memory_learn->lcd.setCursor(0, 0);
  memory_learn->lcd.print("\5     Error    \5");
  memory_learn->lcd.setCursor(0, 1);
  memory_learn->lcd.print(error_message);
  while (memory_learn->buttons) {
    update_buttons(memory_learn);
  }
  ESP.restart();
}

void memory_learn_set_state(MemoryLearn* memory_learn, MemoryLearnState state) {
  memory_learn->state = state;
  memory_learn->lcd.clear();
  Serial.print("Setting MemoryLearn state to: ");
  Serial.println((unsigned char)state);
  switch (state) {
    case MemoryLearnState::BOOT:
      memory_learn_boot(memory_learn);
      break;
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
      memory_learn_error(memory_learn, "Unexpected State");
      break;
  }
}

void memory_learn_init(MemoryLearn *memory_learn) {
  memory_learn_set_state(memory_learn, MemoryLearnState::BOOT);
}

void memory_learn_update(MemoryLearn *memory_learn) {
  update_buttons(memory_learn);
  switch (memory_learn->state) {
    case MemoryLearnState::SELECT_GAME:
      select_game_update(memory_learn);
      break;
    case MemoryLearnState::SIMON:
      simon_game_update(memory_learn);
      break;
    case MemoryLearnState::ABOUT:
      about_update(memory_learn);
      break;
    default:
      memory_learn_error(memory_learn, "Unexpected State");
      break;
  }
}
