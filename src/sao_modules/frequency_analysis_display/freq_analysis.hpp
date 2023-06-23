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
    void draw();
    void setup() {

        // Set WiFi to station mode and disconnect from an AP if it was previously connected
        WiFi.mode(WIFI_STA);
        WiFi.disconnect();
        delay(100);

        Serial.println("Setup done");

    }

    void teardown() {
        return; // no memory to free
    }

    void update()
    {

        activeDisplay->writeTextToScreen("Scan Start", 0, 0);
        activeDisplay->flush();

        int num;
        while(num = WiFi.scanNetworks())
        {
            activeDisplay->flush();
        
            // WiFi.scanNetworks will return the number of networks found
            //int num = WiFi.scanNetworks();

            if (num == 0) {
                activeDisplay->writeTextToScreen("No Networks Found", 0, 0);
                activeDisplay->flush();
            } else {
            String networks = String(num) + " Networks Found";
            activeDisplay->writeTextToScreen(networks, 0, 0);

            for (int i = 0; i < num; ++i) {

                Serial.println(WiFi.RSSI(i));
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
                    activeDisplay->writeTextToScreen(String(WiFi.RSSI(i)), 0, 30);
                }
                activeDisplay->writeTextToScreen(String(WiFi.RSSI(i)), j, 30);
                j += 100;
            }

            activeDisplay->flush();
            }
        }
    }
};

#endif