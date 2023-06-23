
#ifndef SSD1306_128x32_H
#define SSD1306_128x32_H

#include "SSD1306Wire.h" 
#include "./ssd1306_parent.hpp"

class SSD1306_128x32 : public SSD1306 {

private:
    SSD1306Wire display = 0;
    int DISPLAY_WIDTH_MAX = 128;
    int DISPLAY_HEIGHT_MAX = 32;

public:
    SSD1306_128x32() { }

    SSD1306Wire* getDisplay() {
        return new SSD1306Wire(0x3c, SDA, SCL, GEOMETRY_128_32);
    }
};

#endif