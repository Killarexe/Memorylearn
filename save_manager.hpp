#ifndef SAVE_MANAGER_HPP
#define SAVE_MANAGER_HPP

#include "memory_learn.hpp"

#define EEPROM_SIZE 10

void init_save();

uint16_t read_save(uint8_t address);

void write_save(uint8_t address, uint16_t value);

#endif