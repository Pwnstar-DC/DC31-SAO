#ifndef FREQ_ANALYSIS_C
#define FREQ_ANALYSIS_C

#include "freq_analysis.hpp"


void FrequencyAnalysisDisplayModule::setup() {

    // Set WiFi to station mode and disconnect from an AP if it was previously connected
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    delay(100);
    activeDisplay->clear();
    if(SERIAL_ENABLE) {
        Serial.println("Frequency Module: Setup done");
    }
    activeDisplay->flush();
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

void FrequencyAnalysisDisplayModule::logicUpdate() {
    WiFi.scanDelete();
    WiFi.scanNetworks(true, true, 10000);
    delay(10);

    int i = 0;
    String scanningText = "Scanning...";
    int dispWidth = activeDisplay->getRelativeMaxWidth();
    int widthOfScanText = activeDisplay->getWidthOfText(scanningText);
    while(WiFi.scanComplete() == -1) {
        activeDisplay->drawProgress(scanningText, i, 0, 0, dispWidth - widthOfScanText, activeDisplay->getFontOffsetCharHeight());
        activeDisplay->flush();
        i += 1;
        delay(10);
        if(i == 100) {
            i = 0;
        }
    }
    availableNetworks = WiFi.scanComplete();

    if(SERIAL_ENABLE) {
        Serial.println("Scanned networks; found: " + String(availableNetworks));
    }
}

void FrequencyAnalysisDisplayModule::displayUpdate()
{        
    int yOffset = 0;

    activeDisplay->writeTextToScreen("Networks in range: " + String(availableNetworks), 0, yOffset);
    yOffset += activeDisplay->getFontOffsetCharHeight();

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
    if(SERIAL_ENABLE) {
        Serial.println("Wrote networks to display" + String(availableNetworks));
    }
}


 void FrequencyAnalysisDisplayModule::printNetworkInformation(int yOffset) {
    // print header
    // networkName : type : sigStrength : channel
    String fmtStringBase = "%s :: %s :: %s :: %s";
    int bufSize = 512;
    char *buf = (char *) malloc(sizeof(char) * bufSize);
    sprintf(buf, fmtStringBase.c_str(), "SSID", "Enc", "Pow", "Ch");
    activeDisplay->writeTextToScreen(String(buf), 0, yOffset);
    yOffset += getYOffsetIncrement();
    for(int i = 0; i < availableNetworks; i++) {
        // clear buff
        std::memset(buf, 0, bufSize);
        // write line
        sprintf(
            buf, 
            fmtStringBase.c_str(), 
            String(WiFi.SSID(i)),
            String(WiFi.encryptionType(i)),
            String(WiFi.RSSI(i)),
            String(WiFi.channel(i))
        );
        activeDisplay->writeTextToScreen(String(buf), 0, yOffset);
        yOffset += getYOffsetIncrement();
    }
    free(buf);
}


#endif
