/*
 * Troy's HD44780U Lcd Display Emulator
 *
 * Copyright (c) 2020 Troy Schrapel
 *
 * This code is licensed under the MIT license
 *
 * https://github.com/visrealm/VrEmuLcd
 *
 */

#ifndef _VR_EMU_LCD_H_
#define _VR_EMU_LCD_H_

#include <stdint.h>
#include "system_types.h"

/* PRIVATE DATA STRUCTURE
 * ---------------------------------------- */
struct vrEmuLcd_s;
typedef struct vrEmuLcd_s VrEmuLcd;

/* PUBLIC CONSTANTS
 * ---------------------------------------- */
const uint8_t LCD_CMD_CLEAR;
const uint8_t LCD_CMD_HOME;

const uint8_t LCD_CMD_ENTRY_MODE;
const uint8_t LCD_CMD_ENTRY_MODE_INCREMENT;
const uint8_t LCD_CMD_ENTRY_MODE_DECREMENT;
const uint8_t LCD_CMD_ENTRY_MODE_SHIFT;

const uint8_t LCD_CMD_DISPLAY;
const uint8_t LCD_CMD_DISPLAY_ON;
const uint8_t LCD_CMD_DISPLAY_CURSOR;
const uint8_t LCD_CMD_DISPLAY_CURSOR_BLINK;

const uint8_t LCD_CMD_SHIFT;
const uint8_t LCD_CMD_SHIFT_CURSOR;
const uint8_t LCD_CMD_SHIFT_DISPLAY;
const uint8_t LCD_CMD_SHIFT_LEFT;
const uint8_t LCD_CMD_SHIFT_RIGHT;

const uint8_t LCD_CMD_FUNCTION;
const uint8_t LCD_CMD_FUNCTION_LCD_1LINE;
const uint8_t LCD_CMD_FUNCTION_LCD_2LINE;

const uint8_t LCD_CMD_SET_CGRAM_ADDR;
const uint8_t LCD_CMD_SET_DRAM_ADDR;

typedef enum
{
    EmuLcdRomA00,  // Japanese
    EmuLcdRomA02   // European
} vrEmuLcdCharacterRom;

/* PUBLIC INTERFACE
 * ---------------------------------------- */

/* Function:  vrEmuLcdNew
 * --------------------
 * create a new LCD
 *
 * cols: number of display columns  (8 to 40)
 * rows: number of display rows (1, 2 or 4)
 * rom:  character rom to load
 */
VrEmuLcd* vrEmuLcdNew(int width, int height, vrEmuLcdCharacterRom rom);

/* Function:  vrEmuLcdDestroy
 * --------------------
 * destroy an LCD
 *
 * lcd: lcd object to destroy / clean up
 */
void vrEmuLcdDestroy(VrEmuLcd* lcd);

/* Function:  vrEmuLcdSendCommand
 * --------------------
 * send a command to the lcd (RS is low, R/W is low)
 *
 * data: the data (DB0 -> DB7) to send
 */
void vrEmuLcdSendCommand(VrEmuLcd* lcd, uint8_t data);


/* Function:  vrEmuLcdWriteByte
 * --------------------
 * write a byte to the lcd (RS is high, R/W is low)
 *
 * data: the data (DB0 -> DB7) to send
 */
void vrEmuLcdWriteByte(VrEmuLcd* lcd, uint8_t data);

/* Function:  vrEmuLcdWriteString
 * ----------------------------------------
 * write a string to the lcd
 * iterates over the characters and sends them individually
 *
 * str: the string to write.
 */
void vrEmuLcdWriteString(VrEmuLcd* lcd, const char *str);


/* Function:  vrEmuLcdGetDataOffset
 * ----------------------------------------
 * return the character offset in ddram for a given
 * row and column on the display.
 *
 * can be used to set the current cursor address
 */
int vrEmuLcdGetDataOffset(VrEmuLcd* lcd, int row, int col);

/* Function:  vrEmuLcdReadByte
 * --------------------
 * read a byte from the lcd (RS is high, R/W is high)
 *
 * returns: the data (DB0 -> DB7) at the current address
 */
uint8_t vrEmuLcdReadByte(VrEmuLcd* lcd);

/* Function:  vrEmuLcdReadByteNoInc
 * --------------------
 * read a byte from the lcd (RS is high, R/W is high)
 * don't update the address/scroll
 *
 * returns: the data (DB0 -> DB7) at the current address
 */
uint8_t vrEmuLcdReadByteNoInc(VrEmuLcd* lcd);

/* Function:  vrEmuLcdReadAddress
 * --------------------
 * read the current address offset (RS is low, R/W is high)
 *
 * returns: the current address offset (either CGRAM or DDRAM)
 */
uint8_t vrEmuLcdReadAddress(VrEmuLcd* lcd);


/* Function:  vrEmuLcdCharBits
 * ----------------------------------------
 * return a character's pixel data
 *
 * pixel data consists of 5 uint8_ts where each is
 * a vertical row of bits for the character
 *
 * c: character index
 *    0 - 15   cgram
 *    16 - 255 rom
 */
const uint8_t *vrEmuLcdCharBits(VrEmuLcd* lcd, uint8_t c);

/* Function:  vrEmuLcdUpdatePixels
 * ----------------------------------------
 * updates the display's pixel data
 * changes are only reflected in the pixel data when this function is called
 */
void vrEmuLcdUpdatePixels(VrEmuLcd* lcd);

/* Function:  vrEmuLcdNumPixels
 * ----------------------------------------
 * get the size of the entire display in pixels (including unused border pixels)
 */
void vrEmuLcdNumPixels(VrEmuLcd *lcd, int* width, int* height);

/* Function:  vrEmuLcdNumPixelsX
 * ----------------------------------------
 * returns: number of horizontal pixels in the display
 */
int vrEmuLcdNumPixelsX(VrEmuLcd *lcd);

/* Function:  vrEmuLcdNumPixelsY
 * ----------------------------------------
 * returns: number of vertical pixels in the display
 */
int vrEmuLcdNumPixelsY(VrEmuLcd *lcd);

/* Function:  charvrEmuLcdPixelState
 * ----------------------------------------
 * returns: pixel state at the given location
 *
 * -1 = no pixel (character borders)
 *  0 = pixel off
 *  1 = pixel on
 *
 */
char vrEmuLcdPixelState(VrEmuLcd *lcd, int x, int y);

void sendCommand(VrEmuLcd*, uint8_t);
void writeByteToLcd(VrEmuLcd*, uint8_t);
void portAWrite(VrEmuLcd*, byte);
void portBWrite(byte);
byte portBRead();

#endif // _VR_EMU_LCD_H_
