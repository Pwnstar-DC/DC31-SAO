#ifndef FREQ_ANALYSIS_H
#define FREQ_ANALYSIS_H

#include "WiFi.h"
#include <Wire.h> 
#include <SPI.h>
#include "./display_drivers/ssd1306_128x64.hpp"
#include "./display_drivers/ssd1306_parent.hpp"

void draw(SSD1306 *display);
void freq_main(SSD1306 *display);
void freq_setup(SSD1306 *display);



#endif