
#ifndef DISPLAYPARENT_H
#define DISPLAYPARENT_H

class DisplayParent {

public:
    virtual void init() = 0;
    virtual int writeTextToScreen(String text, int locX, int locY) = 0;
    virtual int writeTextToScreen(String text, int locX, int locY, int maxX) = 0;
    virtual void scrollText(String text, int locX, int locY) = 0;
    virtual void flush() = 0;
    virtual void flipScreenVertically() = 0;
    virtual void mirrorDisplay() = 0;
};

#endif