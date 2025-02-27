#include "memory_learn.hpp"

MemoryLearn memory_learn;

void setup() {
  memory_learn_init(&memory_learn);
}

void loop() {
  memory_learn_update(&memory_learn);
}
