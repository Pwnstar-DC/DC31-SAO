#ifndef DISPLAY_MODULE_BASE_H
#define DISPLAY_MODULE_BASE_H

#include "../display_drivers/display_parent.hpp"

class ModuleBase 
{
    // class variables

    // private members
private:
    bool isActive = false;
    bool isSessionPersistent = false;

    // public members
public:

    DisplayParent *activeDisplay;

    ModuleBase(DisplayParent *dispP) {
        activeDisplay = dispP;
    }

    virtual void setup() {};
    virtual void update() {};
    virtual void teardown() {}
    virtual bool getIsSessionPersistent() {
        return isSessionPersistent;
    };

    virtual bool getIsActive() {
        return isActive;
    }
};


#endif