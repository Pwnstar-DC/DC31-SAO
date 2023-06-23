
#ifndef SSD1306_128x64_H
#define SSD1306_128x64_H

#include <Wire.h>
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

    SSD1306Wire* getDisplay() {
        return new SSD1306Wire(0x3c, 5, 4, GEOMETRY_128_64);
    }
};

#endif