
#ifndef DISPLAYPARENT_H
#define DISPLAYPARENT_H

class DisplayParent {

public:
    virtual void init() {};
    virtual int writeTextToScreen(String text, int locX, int locY) {};
    virtual int writeTextToScreen(String text, int locX, int locY, int maxX) {};
    virtual void scrollText(String text, int locX, int locY) {};
    virtual void flush() {};
    virtual void flipScreenVertically() {};
    virtual void mirrorDisplay() {};
};

#endif