#ifndef FREQ_ANALYSIS_C
#define FREQ_ANALYSIS_C

#include "freq_analysis.hpp"


void FrequencyAnalysisDisplayModule::setup() {

    // Set WiFi to station mode and disconnect from an AP if it was previously connected
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    delay(100);

    if(SERIAL_ENABLE) {
        Serial.println("Setup done");
    }
    activeDisplay->writeTest("Scan Start", 0, 0);
    activeDisplay->flush();
    logicUpdate();
    displayUpdate();
}

void FrequencyAnalysisDisplayModule::teardown() {
    return; // no memory to free or configs to clear
}

void FrequencyAnalysisDisplayModule::draw() 
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

 void FrequencyAnalysisDisplayModule::printNetworkInformation(int yOffset) {
    // print header
    // networkName : type : sigStrength : channel
    String fmtStringBase = "%s\t%s\t%s\t%s";
    int bufSize = 512;
    char buf[bufSize];
    sprintf(buf, fmtStringBase.c_str(), "SSID", "Encryption", "Strength", "Channel");
    activeDisplay->writeTextToScreen(buf, 0, yOffset);
    yOffset += getYOffsetIncrement();
    for(int i = 0; i < availableNetworks; i++) {
        // clear buff
        std::memset(buf, 0, bufSize);
        // write line
        sprintf(
            buf, 
            fmtStringBase.c_str(), 
            WiFi.SSID(0),
            WiFi.encryptionType(0),
            WiFi.RSSI(0),
            WiFi.channel(0)
        );
        activeDisplay->writeTextToScreen(buf, 0, yOffset);
        yOffset += getYOffsetIncrement();
    }
}

void FrequencyAnalysisDisplayModule::logicUpdate() {
    availableNetworks = WiFi.scanNetworks();
    if(SERIAL_ENABLE) {
        Serial.println("Scanned networks; found: " + String(availableNetworks));
    }
}

void FrequencyAnalysisDisplayModule::displayUpdate()
{        
    int yOffset = 0;

    activeDisplay->clear();
    activeDisplay->writeTextToScreen("Networks in range: " + String(availableNetworks), 0, yOffset);
    activeDisplay->flush();
    yOffset += getYOffsetIncrement();

    if(availableNetworks <= 0) {
        return;
    }

    switch(mode) {
        case names:
            printNetworkInformation(yOffset);
            break;
        case sigGraph:

            break;
        
    }
    activeDisplay->flush();
}

#endif
