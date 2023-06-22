
#ifndef SSD1306_128x64_H
#define SSD1306_128x64_H

#include "SSD1306Wire.h" 
#include "./ssd1306_parent.hpp"

class SSD1306_128x64 : public SSD1306 {

private:
    SSD1306Wire display = 0;
    int DISPLAY_WIDTH_MAX = 128;
    int DISPLAY_HEIGHT_MAX = 64;

public:
    SSD1306_128x64() {
     }

    SSD1306Wire getDisplay() {
        return SSD1306Wire(0x3c, SDA, SCL, GEOMETRY_128_64);
    }
};

#endif