#include <stdint.h>
#include <math.h>
#include <stdbool.h>
#include "vrEmuLcd.h"
#include "SFML/Graphics.h"
#include "system.h"

#define LCD_HEIGHT 2
#define LCD_WIDTH 16

sfMutex* mutex;
byte running;

sfColor getColor(byte r, byte g, byte b, byte a) {
    sfColor color = {r, g, b, a};
    return color;
}

void renderThread(void* renderWindow) {
    sfRenderWindow* window = (sfRenderWindow*) renderWindow;
    sfRenderWindow_setActive(window, sfTrue);
    uint64_t renderDuration = ((getTSCFrequency() * 1000000) / 60);     //30 Hz
    VrEmuLcd* lcd = get_lcd();
    sfRectangleShape* lcdScreen = sfRectangleShape_create();
    sfVector2f lcdScreenSize = {380, 68}, lcdScreenPos = {10, 10}, pixelSize = {3, 3}, pixelPos;
    sfColor lightBlue = {31, 31, 255, 255};
    sfRectangleShape_setSize(lcdScreen, lcdScreenSize);
    sfRectangleShape_setPosition(lcdScreen, lcdScreenPos);
    sfRectangleShape_setFillColor(lcdScreen, sfTransparent);
    sfRectangleShape* pixels[vrEmuLcdNumPixelsX(lcd)][vrEmuLcdNumPixelsY(lcd)];

    for (int y = 0; y < vrEmuLcdNumPixelsY(lcd); ++y) {
        for (int x = 0; x < vrEmuLcdNumPixelsX(lcd); ++x) {
            pixels[x][y] = sfRectangleShape_create();
            sfRectangleShape_setSize(pixels[x][y], pixelSize);
            pixelPos.x = sfRectangleShape_getPosition(lcdScreen).x + x * 4;
            pixelPos.y = sfRectangleShape_getPosition(lcdScreen).y + y * 4;
            sfRectangleShape_setPosition(pixels[x][y], pixelPos);
            sfRectangleShape_setFillColor(pixels[x][y], sfWhite);
        }
    }

    if(sfRenderWindow_isOpen(window)) {
        sfRenderWindow_clear(window, lightBlue);
        sfRenderWindow_drawRectangleShape(window, lcdScreen, NULL);
        sfRenderWindow_display(window);
    }

    uint64_t renderStartTimePoint = __builtin_ia32_rdtsc();
    while(running) {
        if(getFirstReset()) {
            sfMutex_lock(mutex);
            execute(1);
            sfMutex_unlock(mutex);
        }

        if((__builtin_ia32_rdtsc() - renderStartTimePoint) > renderDuration) {
            sfRenderWindow_clear(window, lightBlue);
            sfRenderWindow_drawRectangleShape(window, lcdScreen, NULL);
            vrEmuLcdUpdatePixels(lcd);

            for (int y = 0; y < vrEmuLcdNumPixelsY(lcd); ++y) {
                for(int x = 0; x < vrEmuLcdNumPixelsX(lcd); ++x) {
                    sfRectangleShape* pixel = pixels[x][y];
                    uint8_t red = sfRectangleShape_getFillColor(pixels[x][y]).r;
                    if(y == 8 || x % 6 == 5) {
                        sfRectangleShape_setFillColor(pixel, lightBlue);
                        continue;
                    }
                    if(!getFirstReset()) {
                        if(y < 8)
                            sfRectangleShape_setFillColor(pixel, sfWhite);
                        else
                            sfRectangleShape_setFillColor(pixel, getColor(0, 0, 224, 100));
                    } else {
                        char pixelState = vrEmuLcdPixelState(lcd, x, y);
                        if(pixelState == -1 || pixelState == 0) {   //Should not be on so turn off if not off already
                            if(red != 0) {
                                double rg = red * .88;
                                sfRectangleShape_setFillColor(pixel, getColor(rg, rg, 224, 225));
                            } else {
                                sfRectangleShape_setFillColor(pixel, getColor(0, 0, 224, 225));
                            }
                        } else if(red != 255) {
                            uint16_t rg = fmin(255, (red + 20) * 2);
                            sfRectangleShape_setFillColor(pixel, getColor(rg, rg, 255, 255));
                        } else {
                            sfRectangleShape_setFillColor(pixel, sfWhite);
                        }
                    }
                    sfRenderWindow_drawRectangleShape(window, pixels[x][y], NULL);
                }
            }
            sfRenderWindow_display(window);
            renderStartTimePoint = __builtin_ia32_rdtsc();
        }
    }
    for (int y = 0; y < vrEmuLcdNumPixelsY(lcd); ++y) {
        for (int x = 0; x < vrEmuLcdNumPixelsX(lcd); ++x) {
            sfRectangleShape_destroy(pixels[x][y]);
        }
    }
    sfRectangleShape_destroy(lcdScreen);
    sfRenderWindow_setActive(window, sfFalse);
}

int main(int argc, char* argv[]) {
    initializeSystem("a.out", .001);
    sfVideoMode mode = {400, 88};
    sfRenderWindow* window = sfRenderWindow_create(mode, "W65C02 Emulation", sfClose, NULL);
    sfEvent event;
    mutex = sfMutex_create();
    sfRenderWindow_setKeyRepeatEnabled(window, sfFalse);
    running = sfTrue;
    sfThread* thread = sfThread_create(renderThread, window);
    sfRenderWindow_setActive(window, sfFalse);
    sfThread_launch(thread);

    sfTime delayTime = sfMilliseconds(1);

    while(running) {
        sfSleep(delayTime);
        while(sfRenderWindow_pollEvent(window, &event)) {
            if(event.type == sfEvtClosed)
                running = false;
            if(event.type == sfEvtKeyPressed) {
                if(event.key.code == sfKeyI) {
                    cpu.IRQB = 1;
                    cpu.IRQB_COMPLETED = 1;
                }
                else if(event.key.code == sfKeyN)
                    cpu.NMIB = 1;
                else if(event.key.code == sfKeyR) {
                    sfMutex_lock(mutex);
                    setFirstReset(1);
                    reset(eeprom[0xFFFC - 0x8000] | eeprom[0xFFFD - 0x8000] << 8);
                    sfMutex_unlock(mutex);
                }
            }
            if(event.type == sfEvtKeyReleased) {
                if(event.key.code == sfKeyI)
                    cpu.IRQB = 0;
                else if(event.key.code == sfKeyN) {
                   cpu.NMIB = 0;
                }
            }
        }
    }

    sfThread_wait(thread);
    sfThread_destroy(thread);
    sfRenderWindow_setActive(window, sfTrue);
    sfMutex_destroy(mutex);
    sfRenderWindow_close(window);
    sfRenderWindow_destroy(window);
    destroySystem();
    return 0;
}
