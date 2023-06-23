#ifndef SSD1306_PARENT_H
#define SSD1306_PARENT_H

#include "SSD1306Wire.h" 
#include "./display_parent.hpp"

class SSD1306 : public DisplayParent {

private:
    SSD1306Wire* display = 0;
    int DISPLAY_WIDTH_MAX = 0;
    int DISPLAY_HEIGHT_MAX = 0;

public:
    SSD1306() {
        
    }

    virtual SSD1306Wire* getDisplay() {
        #ifndef SCLPIN
        #define SCLPIN SCL
        #endif
        #ifndef SDAPIN
        #define SDAPIN SDA
        #endif

        return new SSD1306Wire(0x3c, int(SCLPIN), int(SDAPIN));
    };

    void init() {
        display = getDisplay(); 
        display->init();
        display->setFont(ArialMT_Plain_10);
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

    void write_test(String text, int locX, int locY)
    {
        display->setLogBuffer(1, 30);
        display->println(text);
        display->drawLogBuffer(locX, locY);
        display->display();
        delay(500);
        display->flush();
    }

    void display_current()
    {
        display->display();
    }

    int writeTextToScreen(String text, int locX, int locY) {
        return display->drawStringMaxWidth(locX, locY, getRelativeMaxWidth(), text );
    }

    int writeTextToScreen(String text, int locX, int locY, int maxW) {
        return display->drawStringMaxWidth(locX, locY, maxW, text);
    }

    int getRelativeMaxWidth() {
        return display->getWidth();
    }

    int getRelativeMaxHeight() {
        return display->getHeight();
    }

    int getRelativeWidthOfString(String s) {
        return display->getStringWidth(s);
    }

    void scrollText(String text, int locX, int locY) {
        return;
    }

    void drawVLine(int locX, int locY, int len) {
        display->drawVerticalLine(locX, locY, len);
    }
};

#endif
