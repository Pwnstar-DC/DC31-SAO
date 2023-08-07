#ifndef FREQ_ANALYSIS_C
#define FREQ_ANALYSIS_C

#include "freq_analysis.hpp"

void FrequencyAnalysisDisplayModule::setup() {
    if(SERIAL_DEBUG) {
        Serial.println("Frequency Module: Setup start");
    }
    setCpuFrequencyMhz(80); // minimum required to operate this module
    delay(25);
    esp_wifi_start();
    setDisplayRefreshTime(1500);
    setLogicRefreshTime(50);
    setMetaLogicUpdateTime(15000);
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
    if(SERIAL_DEBUG) {
        Serial.println("Tearing down wifi module");
    }
    // netLineVector contains unique_ptrs, so no need to free

    // power saving features
    esp_wifi_stop();
    setCpuFrequencyMhz(getXtalFrequencyMhz()); // reset mhz to lowest possible
    if(SERIAL_DEBUG) {
        Serial.println("Wifi module teardown complete");
    }
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

void FrequencyAnalysisDisplayModule::logicUpdate(int64_t lastMetaLogicUpdate) {
    if(lastMetaLogicUpdate == 0 || (!scannedAllChannels && readLastScan)) {
        scannedAllChannels = false;
        rebuildNetworkInformation = true;
        readLastScan = false;
        WiFi.scanDelete();
        WiFi.scanNetworks(true, true, false, 10000, currentChannel);
        scanProgressDisplay = 0;
        // clear the area specifically around the scanning text, not entire screen
        activeDisplay->clear(0, 0, dispWidth, fontHeight);
        activeDisplay->write();
        rebuildNetworkInformation = true;
    }

    if(WiFi.scanComplete() == -1) {
        clearMetaLogicUpdateTime();
        if(scanProgressDisplay <= 100) {
            activeDisplay->drawProgress(scanningText + String(currentChannel), scanProgressDisplay, 0, 0, dispWidth - 12, fontHeight);
            activeDisplay->write();
            scanProgressDisplay += 1;
        }
    }
    else if (WiFi.scanComplete() >= 0){
        currentChannel += 1;
        if(SERIAL_DEBUG) {
            Serial.println("Current channel: " + String(currentChannel));
        }
        if(currentChannel > nChannels) {
            currentChannel = 1;
            scannedAllChannels = true;
            displayLineVector.clear();
            rebuildNetworkInformation = false;
            for(int i = 0; i < netLineVector.size(); i++) {
                if(SERIAL_DEBUG) {
                    Serial.println("Transferring network from netline to displayline vector");
                    Serial.flush();
                }
                // release the items from netLineVector into displayLineVector to prevent auto-freeing
                displayLineVector.addToBuffer(std::move(netLineVector.at(i)));
            }
            if(SERIAL_DEBUG) {
                Serial.println("Clearing the old buffer of networks");
                Serial.flush();
                Serial.println("Length of netline: " + String(netLineVector.size()));
                Serial.println("Length of displayline: " + String(displayLineVector.size()));
            }
            // This buffer is using unique pointers, so no need to free them before clearing
            netLineVector.clear();
            WiFi.scanDelete();
        }
        readLastScan = true;
        if(rebuildNetworkInformation) {
            availableNetworks = WiFi.scanComplete();
            buildNetworkInformation();
            rebuildNetworkInformation = false;
        }
    }
}

void FrequencyAnalysisDisplayModule::displayUpdate(int64_t lastMetaDisplayUpdate)
{        
    if(displayLineVector.empty()) {
        return;
    }

    yOffset = 0;
    activeDisplay->clearDisplayBuffer();
    if(!rebuildNetworkInformation) {
        activeDisplay->writeTextToScreen("Networks in range: " + String(displayLineVector.size()), 0, 0);
        activeDisplay->write();
    }

    switch(mode) {
        case names:
            if(SERIAL_DEBUG) {
                Serial.println("FA: refreshing data in names format");
            }
            yOffset += activeDisplay->getFontOffsetCharHeight(); // leave space for text covered in logic update
            printNetworkInformation(yOffset, dispWidth, fontHeight);
            displayLineVector.shiftForward();
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
        Serial.println("adding to the buffer of networks");
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
        Serial.println("FA: current network standing:: nNetworks: " + String(netLineVector.size()));
        Serial.println("FA: sorting networks by signal strength");
        Serial.flush();
    }
    // last step is to sort the vector so that the networks with the greatest
    // signal strength bubble to the top
    netLineVector.sort(wifiNetworkCompare);
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
    for(int i = 0; i <  displayLineVector.size(); i++) {
        wn = std::shared_ptr<WifiNetwork>(displayLineVector.at(i));
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
