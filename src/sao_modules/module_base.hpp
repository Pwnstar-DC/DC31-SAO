#ifndef DISPLAY_MODULE_BASE_H
#define DISPLAY_MODULE_BASE_H

#include "../display_drivers/display_parent.hpp"
#include <ctime>

class ModuleBase 
{
    // class variables

    // private members
private:
    bool isActive = false;
    bool isSessionPersistent = false;
    time_t lastDisplayRefresh = 0;
    time_t lastLogicRefresh = 0;
    int displayRefreshTime = 10000;
    int logicRefreshTime = 100;
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
        time_t t = std::time(nullptr);

        if(t > (lastLogicRefresh + logicRefreshTime)) {
            lastLogicRefresh = t;
            logicUpdate();
        }
        if(t > (lastDisplayRefresh + displayRefreshTime)) {
            lastDisplayRefresh = t;
            displayUpdate();
        }
    }

    virtual int getYOffsetIncrement() {
        return activeDisplay->getFontOffsetCharHeight() + 2*lineSpaceOffset;
    }

};


#endif