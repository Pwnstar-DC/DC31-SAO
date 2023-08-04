#ifndef FREQ_ANALYSIS_H
#define FREQ_ANALYSIS_H

#include "WiFi.h"
#include <Wire.h> 
#include <SPI.h>
#include "../../sao_modules/module_base.hpp"
#include "../../display_drivers/display_parent.hpp"
#include <cstring>
#include <vector>
#include "../../helpers/scrolling_buffer.hpp"
#include <bits/stdc++.h>
#include <OLEDDisplay.h>
#include <esp_wifi.h>

class WifiNetwork {
public:

    WifiNetwork(
        String _bssid,
        String _ssid,
        wifi_auth_mode_t _encType,
        int32_t _dbPower,
        int32_t _channel
    ){
        bssid = _bssid;
        ssid = _ssid;
        if(ssid.equals("?")) {
            ssid = "<hidden>";
        }
        if(ssid.length() > 128) {
            ssid = ssid.substring(0, 125) + "...";
        }
        encType = _encType;
        dbPower = _dbPower;
        // obviously unrealistic, may be due to buffer corruption
        if(dbPower > 512) {
            dbPower = -512;
        }
        channel = _channel;
        if(channel > 512) {
            channel = -512;
        }
    }
    String ssid;
    String bssid;
    wifi_auth_mode_t encType;
    int32_t dbPower;
    int32_t channel;
};

class FrequencyAnalysisDisplayModule : public ModuleBase {

private:
    ScrollingBuffer<std::shared_ptr<WifiNetwork>> netLineVector;
    ScrollingBuffer<std::shared_ptr<WifiNetwork>> displayLineVector;
    uint32_t counter = 0;
    int availableNetworks = 0;
    enum Modes { names, sigGraph };
    Modes mode = names;
    int scanProgressDisplay = 0;
    String scanningText = "Scanning...";
    int dispWidth = 0;
    int dispHeight = 0;
    int widthOfScanText = 0;
    int fontHeight = 0;
    bool rebuildNetworkInformation = false;
    bool scannedAllChannels = false;
    bool readLastScan = false;
    int yOffset = 0;
    int nChannels = 14;
    int currentChannel = 1;
public:
    

    FrequencyAnalysisDisplayModule(DisplayParent *display, String name) : ModuleBase(display, name) {
        dispWidth = activeDisplay->getWidth();
        dispHeight = activeDisplay->getHeight();
        widthOfScanText = activeDisplay->getWidthOfText(scanningText);
        fontHeight = activeDisplay->getFontOffsetCharHeight();
    };
    
    void draw();

    void setup();

    void printNetworkInformation(int yOffset, int dispWidth, int fontHeight);

    void buildNetworkInformation();

    void teardown();

    void logicUpdate(int64_t lastMetaLogicUpdate);

    void displayUpdate(int64_t lastMetaDisplayUpdate);

    void displaySplashScreen();

};

#endif