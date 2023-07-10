#ifndef FREQ_ANALYSIS_C
#define FREQ_ANALYSIS_C

#include "freq_analysis.hpp"

void FrequencyAnalysisDisplayModule::setup() {
    esp_wifi_start();
    setDisplayRefreshTime(1500);
    setLogicRefreshTime(15000);
    // Set WiFi to station mode and disconnect from an AP if it was previously connected
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    delay(100);
    if(SERIAL_DEBUG) {
        Serial.println("Frequency Module: Setup done");
    }
    activeDisplay->flush();
}

void FrequencyAnalysisDisplayModule::teardown() {
    // netLineVector contains unique_ptrs, so no need to free

    // power saving features
    esp_wifi_stop();
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

void FrequencyAnalysisDisplayModule::displaySplashScreen() {
    activeDisplay->clear();
    activeDisplay->writeTextToScreen(
        "2.4g wifi module",
        (activeDisplay->getWidth())/3,
        (activeDisplay->getHeight())/3,
        activeDisplay->getWidth()
    );
    activeDisplay->flush();
    delay(2000);
}

void FrequencyAnalysisDisplayModule::logicUpdate() {
    WiFi.scanDelete();
    WiFi.scanNetworks(true, true, 10000);

    int i = 0;
    // clear the area specifically around the scanning text, not entire screen
    String scanningText = "Scanning...";
    int dispWidth = activeDisplay->getWidth();
    int widthOfScanText = activeDisplay->getWidthOfText(scanningText);
    int fontHeight = activeDisplay->getFontOffsetCharHeight();
    activeDisplay->clear(0, 0, dispWidth, fontHeight);
    activeDisplay->write();
    while(WiFi.scanComplete() == -1) {
        activeDisplay->drawProgress(scanningText, i, 0, 0, dispWidth, fontHeight);
        activeDisplay->write();
        i += 1;
        delay(25);
        if(i == 100) {
            i = 0;
        }
    }
    availableNetworks = WiFi.scanComplete();

    if(SERIAL_DEBUG) {
        Serial.println("Scanned networks; found: " + String(availableNetworks));
    }
}

void FrequencyAnalysisDisplayModule::displayUpdate()
{        
    int yOffset = 0;
    int dispWidth = activeDisplay->getWidth();
    int dispHeight = activeDisplay->getHeight();
    int fontHeight = activeDisplay->getFontOffsetCharHeight();
    activeDisplay->clearDisplayBuffer();
    activeDisplay->writeTextToScreen("Networks in range: " + String(availableNetworks), 0, yOffset);
    yOffset += activeDisplay->getFontOffsetCharHeight();

    if(availableNetworks <= 0) {
        return;
    }

    if(!getHasLogicUpdateSinceLastDisplayUpdate()) {
        // if we hit this, we will just want to
        // scroll the buffer of existing data rather
        // than re-build all of it`
        if(SERIAL_DEBUG) {
            Serial.println("FA: Scrolling objects");
        }
        netLineVector.shiftForward();
    }
    else {
        if(SERIAL_DEBUG) {
            Serial.println("FA: Rebuilding objects");
        }
        buildNetworkInformation();
    }

    switch(mode) {
        case names:
            if(SERIAL_DEBUG) {
                Serial.println("FA: refreshing data in names format");
            }
            printNetworkInformation(yOffset, dispWidth, fontHeight);
            break;
        case sigGraph:
            break;
        
    }
    activeDisplay->write(); // writing here to allow for scan overlay with existing records
   
}
 
bool wifiNetworkCompare(std::shared_ptr<WifiNetwork> one, std::shared_ptr<WifiNetwork> other) {
    return one.get()->dbPower > other.get()->dbPower;
}

void FrequencyAnalysisDisplayModule::buildNetworkInformation() {
    if(SERIAL_DEBUG) {
        Serial.println("Clearing the old buffer of networks");
        Serial.flush();
    }

    // This buffer is using unique pointers, so no need to free them before clearing
    netLineVector.getBuffer().clear();

    if(SERIAL_DEBUG) {
        Serial.println("Building new buffer of networks");
        Serial.flush();
    }
    // check that all discovered networks are tracked in the netLineVector
    for(int i = 0; i < availableNetworks; i++) {
        if(WiFi.BSSIDstr(i)) {
                netLineVector.addToBuffer(
                std::shared_ptr<WifiNetwork>(new WifiNetwork(
                WiFi.BSSIDstr(i),
                WiFi.SSID(i),
                WiFi.encryptionType(i),
                WiFi.RSSI(i),
                WiFi.channel(i)
            )));
        }
    }

    if(SERIAL_DEBUG) {
        Serial.println("FA: built a new list containing nNetworks: " + String(netLineVector.size()));
        Serial.println("FA: sorting networks by signal strength");
        Serial.flush();
    }
    // last step is to sort the vector so that the networks with the greatest
    // signal strength bubble to the top
    netLineVector.getBuffer().sort(wifiNetworkCompare);
}

void FrequencyAnalysisDisplayModule::printNetworkInformation(int yOffset, int dispWidth, int fontHeight) {
    String fmtStringBase = "%s : %s : %s";
    int bufSize = 512;
    char *buf = (char *) malloc(sizeof(char) * bufSize);
    // sprintf(buf, fmtStringBase.c_str(), "SSID", "POW", "CH");
    // activeDisplay->writeTextToScreen(String(buf), 0, yOffset, 512);
    // yOffset += getYOffsetIncrement();

    if(SERIAL_DEBUG) {
        Serial.println("FA-names: prepping to print objects");
        Serial.flush();
    }
    std::shared_ptr<WifiNetwork> wn;
    for(int i = 0; i < netLineVector.size(); i++) {
        wn = netLineVector.at(i);
        if(SERIAL_DEBUG) {
            Serial.println("FA-Printing: " + wn->bssid);
            Serial.flush();
        }
        // clear buff
        std::memset(buf, '\0', bufSize);
        // write line
        sprintf(
            buf, 
            fmtStringBase.c_str(), 
            String(wn->ssid),
            String(wn->dbPower),
            String(wn->channel)
            // String(wn->encType)
        );
        activeDisplay->writeTextToScreen(String(buf), 0, yOffset, -1);
        yOffset += getYOffsetIncrement();
    }
    if(SERIAL_DEBUG) {
        Serial.println("FA-Done updating screen, freeing str buffer");
        Serial.flush();
    }
    free(buf);
    if(SERIAL_DEBUG) {
        Serial.println("FA-Freed buffer");
        Serial.flush();
    }
}


#endif
