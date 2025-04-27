/* bluetooth_manager.hpp
 * Definie les foncitions pour la partie bluetooth du MemoryLearn.
 */
#ifndef BLUETOOTH_MANAGER_HPP
#define BLUETOOTH_MANAGER_HPP

#define DEVICE_NAME "MemoryLearn"

#define SERVICE_UUID        "5ead168e-c598-4c99-bae8-6e7d35625503"
#define CHARACTERISTIC_UUID "f0d0dfc5-3fc1-4bf3-bbd5-3ac67bc58795"

#include "memory_learn.hpp"

/**
 * Initialize le bluetooth du MemoryLearn
 * @param MemoryLearn* memory_learn
 */
void bluetooth_start(MemoryLearn* memory_learn);

#endif
