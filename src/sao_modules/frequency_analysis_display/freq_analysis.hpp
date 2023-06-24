#ifndef FREQ_ANALYSIS_H
#define FREQ_ANALYSIS_H

#include "WiFi.h"
#include <Wire.h> 
#include <SPI.h>
#include "../../sao_modules/module_base.hpp"
#include "../../display_drivers/display_parent.hpp"
#include <cstring>

class FrequencyAnalysisDisplayModule : public ModuleBase {

private:
    uint32_t data[128];
    uint32_t counter = 0;
    int availableNetworks = 0;
    enum Modes { names, sigGraph };
    Modes mode = names;

public:
    FrequencyAnalysisDisplayModule(DisplayParent *display) : ModuleBase(display) {};
    void draw();

    void setup();

    void printNetworkInformation(int yOffset);

    void teardown();

    void logicUpdate();

    void displayUpdate();
};

#endif