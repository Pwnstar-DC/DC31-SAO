
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
    virtual void flush() {};
    virtual void flipScreenVertically() {};
    virtual void mirrorDisplay() {};
    virtual void clear() {};
};

#endif