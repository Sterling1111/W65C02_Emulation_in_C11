#ifndef SYSTEMLIB_SYSTEM_H
#define SYSTEMLIB_SYSTEM_H

#include "W65C02.h"

void initializeSystem(const char*, double);
void destroySystem();
byte getFirstReset();
void setFirstReset(byte);

#endif //SYSTEMLIB_SYSTEM_H
