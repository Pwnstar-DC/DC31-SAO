
#ifndef DISPLAYPARENT_H
#define DISPLAYPARENT_H
#include <Arduino.h>
class DisplayParent {

public:
    virtual void init() {};
    virtual void writeTest(String text, int locX, int locY){};
    virtual void drawBitmap(const unsigned char myBitmap[]);
    virtual int writeTextToScreen(String text, int locX, int locY) {
        return 0;
    };
    virtual int writeTextToScreen(String text, int locX, int locY, int maxX) {
        return 0;
    };
    virtual void drawRect(int locX, int locY, int width, int height);
    virtual void scrollText(String text, int locX, int locY) {};
    virtual void drawVLine(int locX, int locY, int len) {};
    // write is similar to flush but, instead, does not clear the
    // entire display when updated. This makes it useful for making
    // selective modifications to the display and updating.
    virtual void write() {};
    virtual void flush() {};
    virtual void flipScreenVertically() {};
    virtual void mirrorDisplay() {};
    virtual void drawProgress(uint8_t prog, int locX, int locY, int width, int height) {};
    virtual void drawProgress(String txt, uint8_t prog, int locX, int locY, int width, int height) {};
    virtual void clear() {};
    // this implementation allows for clearing specific parts of the screen
    virtual void clear(int locX, int locY, int width, int height) {};
    virtual void clearDisplayBuffer() {};
    virtual uint8_t getFontOffsetCharHeight() {
        return 0;
    };
    virtual int getWidth() { return 0; };
    virtual int getHeight() { return 0; };
    virtual int getWidthOfText(String s) { return 0; };
    virtual void setFont(uint8_t *f) {};

    virtual void setDisplayOff() {};
    virtual void setDisplayOn() {};
    virtual int getColor() { return 0; };
    virtual void setColor(int c) {};
    virtual void setBrightness(int b) {};
};

#endif