#ifndef MODULE_MANAGER_H
#define MODULE_MANAGER_H

#include "../sao_modules/module_base.hpp"
#include "../sao_modules/frequency_analysis_display/freq_analysis.hpp"
#include "../display_drivers/display_parent.hpp"
#include <vector>

class ModuleManager {

private:
    std::vector<ModuleBase *> REGISTERED_MODULES;
    ModuleBase *activeModule;
    int currentActiveModule = -1;

    void writeToSerial(String s) {
        Serial.println("Module Manager: " + s);
    }

public:

    ModuleManager(DisplayParent *dispP) {
        writeToSerial("Registering modules...");
        REGISTERED_MODULES.push_back(
            new FrequencyAnalysisDisplayModule(dispP)
        );
        writeToSerial("Finished registering modules...");
    }

    std::vector<ModuleBase *> getModules();

    int getNumberOfModules();

    int getCurrentActiveModule();

    ModuleBase * getActiveModule();

    void activateModule(int index);

    void deactivateModule(int index);

    void triggerModuleUpdate();

};

#endif

