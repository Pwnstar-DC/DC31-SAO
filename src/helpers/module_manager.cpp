#ifndef MODULE_MANAGER_C
#define MODULE_MANAGER_C

#include "../sao_modules/module_base.hpp"
#include "../sao_modules/frequency_analysis_display/headers.hpp"
#include "../display_drivers/display_parent.hpp"
// assuming all of the pointer stuff here works, may need to revisit

class ModuleManager {

private:
    int nModules = 0;
    ModuleBase *REGISTERED_MODULES;
    ModuleBase activeModule;
    int currentActiveModule = -1;

public:

    ModuleManager(DisplayParent *dispP) {
        REGISTERED_MODULES = {
            new FrequencyDisplayModule(dispP)
        };
    }

    ModuleBase *getModules() {
        return REGISTERED_MODULES;
    }

    int getNumberOfModules() {
        return nModules;
    }

    int getCurrentActiveModule() {
        return currentActiveModule;
    }

    ModuleBase getActiveModule() {
        return activeModule;
    }

    void activateModule(int index) {
        if(!(index >= 0 && index < nModules)) {
            return; // error case
        }

         if(index == currentActiveModule) {
            return;
        }

        ModuleBase targetModule = REGISTERED_MODULES[index];    
        currentActiveModule = index;
        targetModule.setup();
        activeModule = targetModule;
    }

    void deactivateModule(int index) {
        if(!(index >= 0 && index < nModules)) {
            return; // error case
        }

        ModuleBase targetModule = REGISTERED_MODULES[index];
        // if a module is session-persistent, it should be suspended, not disassembled
        if(targetModule.getIsActive() && !targetModule.getIsSessionPersistent()) {
            targetModule.teardown();
        }
    }

    void triggerModuleUpdate() {
        activeModule.update();
    }

};

#endif