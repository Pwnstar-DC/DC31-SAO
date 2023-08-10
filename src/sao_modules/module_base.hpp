#ifndef DISPLAY_MODULE_BASE_H
#define DISPLAY_MODULE_BASE_H

#include "../display_drivers/display_parent.hpp"
#include <ctime>
#include <chrono>

using namespace std::chrono;
class ModuleBase 
{
    // class variables

    // private members
private:
    bool isActive = false;
    bool isSessionPersistent = false;
    bool previouslyActive = false;
    uint32_t lastDisplayRefresh = 0;
    uint32_t lastLogicRefresh = 0;
    uint32_t lastMetaDisplayUpdate = 0;
    uint32_t lastMetaLogicUpdate = 0;
    uint32_t metaDisplayUpdateTime = 10000;
    uint32_t metaLogicUpdateTime = 10000;
    uint32_t displayRefreshTime = 250;
    uint32_t logicRefreshTime = 10000;
    int lineSpaceOffset = 0.5;
    bool logicUpdateSinceLastDisplayUpdate = false;
    String moduleName = "";
    bool firstLoop = true; // stooopid edge case fix

    // public members
public:

    DisplayParent *activeDisplay;

    ModuleBase(DisplayParent *dispP, String _moduleName) {
        activeDisplay = dispP;
        moduleName = _moduleName;
    }

    virtual void setup() {};
    virtual void displayUpdate(uint32_t timeSinceLastMetaDisplayUpdate) {};
    virtual void logicUpdate(uint32_t timeSinceLastMetaLogicUpdate) {};
    virtual void teardown() {}
    virtual bool getIsSessionPersistent() {
        return isSessionPersistent;
    };

    virtual void displaySplashScreen() { };

    bool getHasLogicUpdateSinceLastDisplayUpdate() {
        return logicUpdateSinceLastDisplayUpdate;
    }

    virtual bool getIsActive() {
        return isActive;
    }

    virtual void waitForSync() {

        uint32_t now = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();

        if(now > (lastLogicRefresh + logicRefreshTime)) {
            if(now > (lastMetaLogicUpdate + metaLogicUpdateTime) || firstLoop) {
                lastMetaLogicUpdate = now;
                firstLoop = false; // thanks kevin -- pwnstar
            }

            logicUpdate(now - lastMetaLogicUpdate);
            lastLogicRefresh = now;
            logicUpdateSinceLastDisplayUpdate = true;
        }

        if(now > (lastDisplayRefresh + displayRefreshTime)) {
            if(now > (lastMetaDisplayUpdate + metaDisplayUpdateTime)) {
                lastMetaDisplayUpdate = now;
            }
            
            displayUpdate(now - lastMetaDisplayUpdate);
            lastDisplayRefresh = now;
            logicUpdateSinceLastDisplayUpdate = false;
        }
    }

    virtual int getYOffsetIncrement() {
        if(SERIAL_DEBUG) {
            Serial.println("increment y offset");
            Serial.flush();
        }
        return activeDisplay->getFontOffsetCharHeight() + lineSpaceOffset;
    }

    virtual void cycleMode() { };

    virtual void setDisplayRefreshTime(int r) {
        displayRefreshTime = r;
    }

    virtual void setLogicRefreshTime(int r) {
        logicRefreshTime = r;
    }

    virtual void setMetaDisplayUpdateTime(int r) {
        metaDisplayUpdateTime = r;
    }

    virtual void setMetaLogicUpdateTime(int r) {
        metaLogicUpdateTime = r;
    }

    virtual String getName() {
        return moduleName;
    }

    void deactivate() {
        if(!isSessionPersistent) {
            teardown();
        }
    }

    void resetMetaLogicUpdateTime() {
        lastMetaLogicUpdate = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
    }
    void resetMetaDisplayUpdateTime() {
        lastMetaDisplayUpdate = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
    }

    void clearMetaLogicTime() {
        metaLogicUpdateTime = 0;
    }
    void clearMetaDisplayTime() {
        metaDisplayUpdateTime = 0;
    }
};


#endif