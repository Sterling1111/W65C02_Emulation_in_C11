#ifndef SYSTEMLIB_W65C22_H
#define SYSTEMLIB_W65C22_H

#include "system_types.h"
#include "vrEmuLcd.h"

VrEmuLcd* get_lcd();
void initializeRegisters();
void destroyRegisters();
byte readFromRegisters(word address);
void writeToRegisters(word address, byte data);
void resetW65C22Registers();

#endif //SYSTEMLIB_W65C22_H
