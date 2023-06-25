
#ifndef DISPLAYPARENT_H
#define DISPLAYPARENT_H
#include <Arduino.h>
class DisplayParent {

public:
    virtual void init() {};
    virtual void writeTest(String text, int locX, int locY){};
    virtual int writeTextToScreen(String text, int locX, int locY) {
        return 0;
    };
    virtual int writeTextToScreen(String text, int locX, int locY, int maxX) {
        return 0;
    };
    virtual void scrollText(String text, int locX, int locY) {};
    virtual void drawVLine(int locX, int locY, int len) {};
    virtual void write() {};
    virtual void flush() {};
    virtual void flipScreenVertically() {};
    virtual void mirrorDisplay() {};
    virtual void drawProgress(u8_t prog, int locX, int locY, int width, int height) {};
    virtual void drawProgress(String txt, u8_t prog, int locX, int locY, int width, int height) {};
    virtual void clear() {};
    virtual uint8_t getFontOffsetCharHeight() {
        return 0;
    };
    virtual int getRelativeMaxWidth() { return 0; };
    virtual int getRelativeMaxHeight() { return 0; };
    virtual int getWidthOfText(String s) { return 0; };
    virtual void setFont(uint8_t *f) {};

    virtual void setDisplayOff() {};
    virtual void setDisplayOn() {};
};

#endif