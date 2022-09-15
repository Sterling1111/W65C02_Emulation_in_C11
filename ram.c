#include "ram.h"

byte ram[1024 * 32];

void initializeRam() {
    for (int i = 0; i < 1024 * 32; ++i) {
        ram[i] = 0xea;  //nop
    }
}

byte readRAM(word address) {
    return ram[address];
}

void writeRAM(word address, byte data) {
    ram[address] = data;
}