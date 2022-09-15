#include "bus.h"

void writeBus(word address, byte data) {
    if(address >= 0x00 && address <= 0x3fff)
        writeRAM(address, data);
    else if(address >= 0x6000 && address <= 0x7fff)
        writeToRegisters(address - 0x6000, data);
    else if(address >= 0x8000 && address <= 0xffff)
        return;
}

byte readBus(word address) {
    if(address >= 0x00 && address <= 0x3fff)
        return readRAM(address);
    else if(address >= 0x6000 && address <= 0x7fff)
        return readFromRegisters(address - 0x6000);
    else if(address >= 0x8000 && address <= 0xffff)
        return readEEPROM(address - 0x8000);
}