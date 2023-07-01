#ifndef SSD1306_PARENT_H
#define SSD1306_PARENT_H

#include "SSD1306Wire.h" 
#include "./display_parent.hpp"
#include <iostream>
#include <string>
#include <sstream>
#include "OLEDDisplay.h" // for access to font functions
#include <Adafruit_GFX.h>

class SSD1306 : public DisplayParent {

#ifndef SCLPIN
#define SCLPIN SCL
#endif
#ifndef SDAPIN
#define SDAPIN SDA
#endif
#ifndef DISP_IIC_ADDR
#define DISP_IIC_ADDR 0x3c
#endif
#ifndef SCREEN_X
#define SCREEN_X 128
#endif
#ifndef SCREEN_Y
#define SCREEN_Y 64
#endif

#if SCREEN_X == 128 && SCREEN_Y == 32
#define SCREEN_FMT GEOMETRY_128_32
#elif SCREEN_X == 64 && SCREEN_Y == 48
#define SCREEN_FMT GEOMETRY_64_48
#elif SCREEN_X == 64 && SCREEN_Y == 32
#define SCREEN_FMT GEOMETRY_64_32
#else
#define SCREEN_FMT GEOMETRY_128_64
#endif

private:
    SSD1306Wire* display = 0;
    int DISPLAY_WIDTH_MAX = SCREEN_X;
    int DISPLAY_HEIGHT_MAX = SCREEN_Y;
    int initialized = 0;
    uint8_t *currentFont = (uint8_t *) ArialMT_Plain_10;

public:
    String hexToString(int x) {
        std::stringstream s;
        s << "0x" << std::hex << x;
        return String(s.str().c_str());
    }

    SSD1306() {
    
    }

    SSD1306Wire* getDisplay() {
        if(SERIAL_DEBUG) {
            Serial.println("Using the following display configuration:");
            Serial.println("Screen size: " + String(SCREEN_X) + "x" + String(SCREEN_Y));
            Serial.println("Screen I2C Address: " + hexToString(DISP_IIC_ADDR));
            Serial.println("I2C Com Pins: SDA: " + hexToString(SDAPIN) + ", SCL/SCK: " + hexToString(SCLPIN));
            Serial.println();
            Serial.flush();
        }
        
        return new SSD1306Wire(DISP_IIC_ADDR, SDAPIN, SCLPIN , SCREEN_FMT);
    };

    void init() {
        if(initialized) {
            return;
        }
        initialized = 1;
        display = getDisplay(); 
        display->init();
        display->flipScreenVertically();
        setFont(currentFont);
        display->displayOn();
        display->drawStringMaxWidth(DISPLAY_WIDTH_MAX/2, DISPLAY_HEIGHT_MAX/2, getWidth(), "initialized" );
        display->flush();
    }

    void setDisplayOn() {
        display->displayOn();
    }

    void setDisplayOff() {
        display->displayOff();
    }

    void flipScreenVertically() {
        display->flipScreenVertically();
    }

    void mirrorDisplay() {
        display->mirrorScreen();
    }

    void flush() {
        display->display();
        display->clear();
    }

    void setFont(uint8_t *f) {
        display->setFont(f);
    }

    void writeTest(String text, int locX, int locY)
    {
        display->setLogBuffer(1, 30);
        display->println(text);
        display->drawLogBuffer(locX, locY);
        display->display();
        delay(500);
        display->flush();
    }

    void drawBitmap(const unsigned char myBitmap[])
    {
        display->drawXbm(0, 0, 128, 64, myBitmap);
        //display->drawBitmap(0, 0, myBitmap, 128, 64, 1);
    }

    void write()
    {
        display->display();
    }

    int writeTextToScreen(String text, int locX, int locY) {
        return display->drawString(locX, locY, text );
    }

    int writeTextToScreen(String text, int locX, int locY, int maxW) {
        return display->drawStringMaxWidth(locX, locY, maxW, text);
    }

    int getWidth() {
        return display->getWidth();
    }

    int getHeight() {
        return display->getHeight();
    }

    uint8_t getFontOffsetCharHeight() {
        return pgm_read_byte(currentFont + HEIGHT_POS);
    }

    int getRelativeWidthOfString(String s) {
        return display->getStringWidth(s);
    }

    void scrollText(String text, int locX, int locY) {
        return;
    }

    int getWidthOfText(String text) {
        return display->getStringWidth(text);
    }

    void drawVLine(int locX, int locY, int len) {
        display->drawVerticalLine(locX, locY, len);
    }

    void clear() {
        display->clear();
        display->display();
    }

    void clear(int locX, int locY, int width, int height) {
        for(int i = locX; i < width; i++) {
            for(int j = locY; j < height; j++) {
                display->clearPixel(i, j);
            }
        }
        display->display();
    }

    void clearDisplayBuffer() {
        display->clear();
    }

    void drawProgress(uint8_t prog, int locX, int locY, int width, int height) {
        // clear the buffer where the dimensions overlap
        display->drawProgressBar(locX, locY, width, height, prog);
    }

    void drawProgress(String preText, uint8_t prog, int locX, int locY, int width, int height) { 
        int textWidth = display->getStringWidth(preText) + 1;
        int progWidth = width;
        if((width + textWidth) > display->getWidth()) {
            // subtract width from the progress bar to make space
            progWidth = (width + textWidth) - display->getWidth();
        }
        writeTextToScreen(preText, locX, locY);
        drawProgress(prog, locX + textWidth, locY, progWidth, height);
    }

    void drawRect(int locX, int locY, int width, int height) {
        display->drawRect(locX, locY, width, height);
    }

    int getColor() {
        return display->getColor();
    }

    void setColor(int c) {
        display->setColor((OLEDDISPLAY_COLOR)c);
    }

};

#endif
