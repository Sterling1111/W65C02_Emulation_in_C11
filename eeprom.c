#include "eeprom.h"

byte eeprom[1024 * 32];

byte readEEPROM(word address) {
    return eeprom[address];
}

void loadProgram(const char* programObjFile) {
    FILE* fileptr;
    fileptr = fopen(programObjFile, "rb");
    if(fileptr == NULL) exit(69420);
    fread(eeprom, 1024 * 32, 1, fileptr);
}