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
    int64_t lastDisplayRefresh = 0;
    int64_t lastLogicRefresh = 0;
    int64_t lastMetaDisplayUpdate = 0;
    int64_t lastMetaLogicUpdate = 0;
    int metaDisplayUpdateTime = 10000;
    int metaLogicUpdateTime = 10000;
    int displayRefreshTime = 250;
    int logicRefreshTime = 10000;
    int lineSpaceOffset = 0.5;
    bool logicUpdateSinceLastDisplayUpdate = false;
    String moduleName = "";

    // public members
public:

    DisplayParent *activeDisplay;

    ModuleBase(DisplayParent *dispP, String _moduleName) {
        activeDisplay = dispP;
        moduleName = _moduleName;
    }

    virtual void setup() {};
    virtual void displayUpdate(int64_t timeSinceLastMetaDisplayUpdate) {};
    virtual void logicUpdate(int64_t timeSinceLastMetaLogicUpdate) {};
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

        int64_t now = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();

        if(now > (lastLogicRefresh + logicRefreshTime)) {
            if(now > (lastMetaLogicUpdate + metaLogicUpdateTime)) {
                lastMetaLogicUpdate = now;
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

    void clearMetaLogicUpdateTime() {
        lastMetaLogicUpdate = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
    }
    void clearMetaDisplayUpdateTime() {
        lastMetaDisplayUpdate = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
    }

};


#endif