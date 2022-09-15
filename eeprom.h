#ifndef CPROJECT_EEPROM_H
#define CPROJECT_EEPROM_H

#include "system_types.h"
#include <stdio.h>
#include <stdlib.h>

extern byte eeprom[1024 * 32];
byte readEEPROM(word);
void loadProgram(const char*);

#endif //CPROJECT_EEPROM_H
