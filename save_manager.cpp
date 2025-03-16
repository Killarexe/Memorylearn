#include "save_manager.hpp"
#include <EEPROM.h>

void init_save() {
  EEPROM.begin(EEPROM_SIZE);
}

uint16_t read_save(uint8_t address) {
  return EEPROM.read(address) << 7 | EEPROM.read(address + 1);
}

void write_save(uint8_t address, uint16_t value) {
  EEPROM.write(address, value >> 7);
  EEPROM.write(address, value & 0xFF);
}