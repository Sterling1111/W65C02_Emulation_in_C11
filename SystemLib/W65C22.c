#include "W65C22.h"

const int LCD_HEIGHT = 2;
const int LCD_WIDTH = 16;

byte registers[16];

VrEmuLcd* lcd;

VrEmuLcd* get_lcd() {
    return lcd;
}

void initializeRegisters() {
    lcd = vrEmuLcdNew(LCD_WIDTH, LCD_HEIGHT, EmuLcdRomA00);
    resetW65C22Registers();
}

void destroyRegisters() {
    vrEmuLcdDestroy(lcd);
}

void resetW65C22Registers() {
    for (int i = 0; i < 16; ++i) {
        registers[i] = 0;
    }
}

void writeToRegisters(word address, byte data) {
    registers[address & 0x0F] = data;
    if((address & 0x0F) == 0) {     //IORB
        byte num = 0;
        for (int i = 0; i < 8; ++i) {
            if(registers[2] & (1 << i)) {   //DDRB
                num |= (data & (1 << i));
            }
        } portBWrite(num);
    } else if((address & 0x0F) == 1) {  //IORA
        byte num = 0;
        for (int i = 0; i < 8; ++i) {
            if(registers[3] & (1 << i)) {   //DDRA
                num |= (data & (1 << i));
            }
        } portAWrite(lcd, num);
    }
}

byte readFromRegisters(word address) {
    if((address & 0xF) == 0) //IORB
        return portBRead();
    return 0;
}