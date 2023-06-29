#ifndef MODULE_MANAGER_H
#define MODULE_MANAGER_H

#include "../sao_modules/module_base.hpp"
#include "../sao_modules/frequency_analysis_display/freq_analysis.hpp"
#include "../display_drivers/display_parent.hpp"
//#include "../sao_modules/snake_module/headers.hpp"
#include "../sao_modules/ble_server/ble_server.hpp"
#include <vector>

class ModuleManager {

private:
    std::vector<ModuleBase *> REGISTERED_MODULES;
    ModuleBase *activeModule = NULL;
    std::vector<DisplayParent *> displays;

    void writeToSerial(String s) {
        if(!SERIAL_DEBUG) {
            return;
        }
        Serial.println("Module Manager: " + s);
    }

public:

    ModuleManager(DisplayParent *dispP) {
        writeToSerial("Registering modules...");
        REGISTERED_MODULES.push_back(
            new FrequencyAnalysisDisplayModule(dispP, "frequency_module")
        );
        //REGISTERED_MODULES.push_back(
        //    new SnakeModule(dispP, "snake_module")
        //);
        REGISTERED_MODULES.push_back(
            new BLEServerModule(dispP, "ble_server_module")
        );
        displays.push_back(dispP);
        writeToSerial("Finished registering modules...");
    }

    std::vector<ModuleBase *> getModules();

    int getNumberOfModules();

    ModuleBase * getActiveModule();

    void activateModule(int index);

    void activateModule(ModuleBase * mod);

    void deactivateModule(int index);

    void triggerModuleUpdate();

    void cycleMode();

    ModuleBase * getModuleByName(String name);

};

#endif

