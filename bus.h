#ifndef CPROJECT_BUS_H
#define CPROJECT_BUS_H

#include "eeprom.h"
#include "ram.h"
#include "W65C22.h"

void writeBus(word, byte);
byte readBus(word);

#endif //CPROJECT_BUS_H
