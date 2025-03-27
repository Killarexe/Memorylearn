/**
 * save_manager.hpp
 * Definition des fonctions pour s'occuper de la partie sauvegarde des scores. (Non fini)
 */
#ifndef SAVE_MANAGER_HPP
#define SAVE_MANAGER_HPP

#include "memory_learn.hpp"

// Taille de la sauvegarde en octées.
#define EEPROM_SIZE 10

/**
 * Initialize la sauvegarde.
 */
void init_save();

/**
 * Lit une valeur 16-bit dans la sauvegarde à une adresse.
 * @param uint8_t address
 * @return une valeur 16-bit.
 */
uint16_t read_save(uint8_t address);

/**
 * Ecrit une valeur 16-bit dans la sauvegarde à une adresse.
 * @param uint8_t address
 * @param uint16_t value valeur
 */
void write_save(uint8_t address, uint16_t value);

#endif
