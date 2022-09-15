#include "system.h"

byte firstReset;

void initializeSystem(const char* programObjFile, double Mhz) {
    initializeRegisters();
    loadProgram(programObjFile);
    reset(eeprom[0xFFFC - 0x8000] | eeprom[0xFFFD - 0x8000] << 8);
    initializeCycleInfo();
    setCycleDuration(Mhz);
    initializeRam();
    firstReset = 0;
}

byte getFirstReset() {
    return firstReset;
}

void setFirstReset(byte state) {
    firstReset = state;
}

void destroySystem() {
    destroyRegisters();
}