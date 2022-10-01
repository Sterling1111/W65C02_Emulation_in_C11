#ifndef CPROJECT_RAM_H
#define CPROJECT_RAM_H

#include "system_types.h"

byte readRAM(word);
void writeRAM(word, byte);
void initializeRam();

#endif //CPROJECT_RAM_H
