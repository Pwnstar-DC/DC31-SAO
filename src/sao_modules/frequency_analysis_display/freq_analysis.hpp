#ifndef FREQ_ANALYSIS_H
#define FREQ_ANALYSIS_H

#include "WiFi.h"
#include <Wire.h> 
#include <SPI.h>
#include "../../sao_modules/module_base.hpp"
#include "../../display_drivers/display_parent.hpp"
#include <cstring>
#include <vector>

class WifiNetwork {
public:

    WifiNetwork(
        String _ssid,
        wifi_auth_mode_t _encType,
        int32_t _dbPower,
        int32_t _channel
    ){
        ssid = _ssid;
        if(ssid.equals("?")) {
            ssid = "<hidden>";
        }
        encType = _encType;
        dbPower = _dbPower;
        channel = _channel;
    }
    String ssid;
    wifi_auth_mode_t encType;
    int32_t dbPower;
    int32_t channel;
};

class FrequencyAnalysisDisplayModule : public ModuleBase {

private:
    std::vector<WifiNetwork *> netLineVector;
    uint32_t counter = 0;
    int availableNetworks = 0;
    enum Modes { names, sigGraph };
    Modes mode = names;

public:
    FrequencyAnalysisDisplayModule(DisplayParent *display) : ModuleBase(display) {};
    void draw();

    void setup();

    void printNetworkInformation(int yOffset, int dispWidth, int fontHeight);

    void teardown();

    void logicUpdate();

    void displayUpdate();
};

#endif