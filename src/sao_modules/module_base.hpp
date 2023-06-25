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
    int64_t lastDisplayRefresh = 0;
    int64_t lastLogicRefresh = 0;
    int displayRefreshTime = 100;
    int logicRefreshTime = 10000;
    int lineSpaceOffset = 1;

    // public members
public:

    DisplayParent *activeDisplay;

    ModuleBase(DisplayParent *dispP) {
        activeDisplay = dispP;
    }

    virtual void setup() {};
    virtual void displayUpdate() {};
    virtual void logicUpdate() {};
    virtual void teardown() {}
    virtual bool getIsSessionPersistent() {
        return isSessionPersistent;
    };

    virtual bool getIsActive() {
        return isActive;
    }

    virtual void waitForSync() {

        int64_t now = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();

        if(now > (lastLogicRefresh + logicRefreshTime)) {
            logicUpdate();
            lastLogicRefresh = now;
        }
        if(now > (lastDisplayRefresh + displayRefreshTime)) {
            displayUpdate();
            lastDisplayRefresh = now;
        }
    }

    virtual int getYOffsetIncrement() {
        return activeDisplay->getFontOffsetCharHeight() + 2*lineSpaceOffset;
    }

};


#endif