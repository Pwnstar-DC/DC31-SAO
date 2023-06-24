#ifndef FREQ_ANALYSIS_H
#define FREQ_ANALYSIS_H

#include "WiFi.h"
#include <Wire.h> 
#include <SPI.h>
#include "../../sao_modules/module_base.hpp"
#include "../../display_drivers/display_parent.hpp"

class FrequencyAnalysisDisplayModule : public ModuleBase {

private:
    byte data[128];

public:
    FrequencyAnalysisDisplayModule(DisplayParent *display) : ModuleBase(display) {};
    void draw()
    {
        int HighestPeak;
        int Height;

        //draw X axis notches:
        for(byte x = 0; x < 128; x+=10)
        {
            //draw notches every 10 ticks to allow identifying the range of frequencies
            //u8g2.drawVLine(x,62,2);
            activeDisplay->drawVLine(x,62,2);
        }
    }

    void setup() {

        // Set WiFi to station mode and disconnect from an AP if it was previously connected
        WiFi.mode(WIFI_STA);
        WiFi.disconnect();
        delay(100);

        Serial.println("Setup done");
        activeDisplay->writeTest("Scan Start", 0, 0);
        activeDisplay->flush();
        update();
    }

    void teardown() {
        return; // no memory to free
    }

    void update()
    {
        int num;
        num = WiFi.scanNetworks();
        activeDisplay->clear();

        // WiFi.scanNetworks will return the number of networks found
        //int num = WiFi.scanNetworks();

        if (num == 0) {
            activeDisplay->writeTest("No Networks Found", 0, 0);
            activeDisplay->flush();
        } else {
            String networks = String(num) + " Networks Found";
            Serial.println(networks);
            activeDisplay->writeTest(networks, 0, 0);

            for (int i = 0; i < num; ++i) {

                Serial.println(WiFi.RSSI(i));
                activeDisplay->writeTest(String(WiFi.RSSI(i)),0,0);
                //display.println(")");
                //display.println((WiFi.encryptionType(i) == WIFI_AUTH_OPEN)?" ":"*");
                data[i] = WiFi.RSSI(i);
                //delay(10);
            }

            int j = 100;
            for(int i=0; i < num; i++)
            {
                if(i=0)
                {
                    activeDisplay->writeTest(String(WiFi.RSSI(i)), 0, 30);
                }
                activeDisplay->writeTest(String(WiFi.RSSI(i)), j, 30);
                j += 100;
            }
            draw();
            activeDisplay->flush();
        }
    }

    void waitForSync() {
        delay(10000);
    }
};

#endif