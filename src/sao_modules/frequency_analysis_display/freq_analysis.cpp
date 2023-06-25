#ifndef FREQ_ANALYSIS_C
#define FREQ_ANALYSIS_C

#include "freq_analysis.hpp"
#include <bits/stdc++.h>


void FrequencyAnalysisDisplayModule::setup() {

    // Set WiFi to station mode and disconnect from an AP if it was previously connected
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    delay(100);
    if(SERIAL_ENABLE) {
        Serial.println("Frequency Module: Setup done");
    }
    activeDisplay->flush();
}

void FrequencyAnalysisDisplayModule::teardown() {
    for(int i = 0; i < netLineVector.size(); i++) {
        free(netLineVector.at(i));
    }
    return;
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

    int i = 0;
    // clear the area specifically around the scanning text, not entire screen
    String scanningText = "Scanning...";
    int dispWidth = activeDisplay->getRelativeMaxWidth();
    int widthOfScanText = activeDisplay->getWidthOfText(scanningText);
    int fontHeight = activeDisplay->getFontOffsetCharHeight();
    activeDisplay->clear(0, 0, dispWidth, fontHeight);
    activeDisplay->write();
    while(WiFi.scanComplete() == -1) {
        activeDisplay->drawProgress(scanningText, i, 0, 0, dispWidth, fontHeight);
        activeDisplay->write();
        i += 1;
        delay(55);
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
    if(!getHasLogicUpdateSinceLastDisplayUpdate()) {
        return;
    }

    int yOffset = 0;
    int dispWidth = activeDisplay->getRelativeMaxWidth();
    int fontHeight = activeDisplay->getFontOffsetCharHeight();
    activeDisplay->clear(0, 0, dispWidth, fontHeight); // clear the scanning text
    activeDisplay->writeTextToScreen("Networks in range: " + String(availableNetworks), 0, yOffset);
    yOffset += activeDisplay->getFontOffsetCharHeight();

    if(availableNetworks <= 0) {
        return;
    }

    switch(mode) {
        case names:
            printNetworkInformation(yOffset, dispWidth, fontHeight);
            break;
        case sigGraph:

            break;
        
    }
    activeDisplay->write(); // writing here to allow for scan overlay with existing records
    if(SERIAL_ENABLE) {
        Serial.println("Wrote networks to display" + String(availableNetworks));
    }
}
 
bool wifiNetworkCompare(WifiNetwork *one, WifiNetwork *other) {
    return one->dbPower > other->dbPower;
}

void FrequencyAnalysisDisplayModule::printNetworkInformation(int yOffset, int dispWidth, int fontHeight) {
    // prep the network objects

    // check that all discovered networks are tracked in the netLineVector
    for(int i = 0; i < availableNetworks; i++) {
        bool foundMatch = false;
        // check if the wifi network exists in the vector already
        for(int j = 0; j < netLineVector.size(); j++) {
            if((*netLineVector.at(j)).ssid.equals(WiFi.SSID(i))) {
                // this implies that we already have the ssid tracked
                foundMatch = true;
                break;
            }
        }
        if(foundMatch) {
            continue;
        }
        // if we don't have a match, we need to add it to the networks
        netLineVector.push_back(new WifiNetwork(
            WiFi.SSID(i),
            WiFi.encryptionType(i),
            WiFi.RSSI(i),
            WiFi.channel(i)
        ));
    }

    // now, we do the same thing but make sure that we dont have anyting
    // leftover in the netLineVector that was not captured in the most recent scan
    for(int i = 0; i < netLineVector.size(); i++) {
        bool foundMatch = false;
        for(int j = 0; j < availableNetworks; j++) {
            if((*netLineVector.at(i)).ssid.equals(WiFi.SSID(j))) {
                // this tells us we should continue to track this one
                foundMatch = true;
                break;
            }
        }
        if(foundMatch) {
            continue;
        }
        // in this case, we need to remove the network from the vector
        // as this implies the network is now out of range
        free(netLineVector.at(i));
        netLineVector.erase(netLineVector.begin()+i);
    }

    // last step is to sort the vector so that the networks with the greatest
    // signal strength bubble to the top
    std::sort(netLineVector.begin(), netLineVector.end(), wifiNetworkCompare);

    String fmtStringBase = "%s : %s : %s : %s";
    int bufSize = 512;
    char *buf = (char *) malloc(sizeof(char) * bufSize);
    sprintf(buf, fmtStringBase.c_str(), "SSID", "POW", "CH", "ENC");
    activeDisplay->writeTextToScreen(String(buf), 0, yOffset, 512);
    yOffset += getYOffsetIncrement();

    for(const auto wn : netLineVector) {
        // clear buff
        std::memset(buf, 0, bufSize);
        // write line
        sprintf(
            buf, 
            fmtStringBase.c_str(), 
            String(wn->ssid),
            String(wn->dbPower),
            String(wn->channel),
            String(wn->encType)
        );
        // overwrite the current line
        activeDisplay->clear(0, yOffset, dispWidth, fontHeight);
        activeDisplay->writeTextToScreen(String(buf), 0, yOffset, -1);
        yOffset += getYOffsetIncrement();
    }
    free(buf);
}


#endif
