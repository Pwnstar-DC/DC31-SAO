#ifndef MODULE_MANAGER_H
#define MODULE_MANAGER_H

#include "../sao_modules/module_base.hpp"
#include "../sao_modules/frequency_analysis_display/freq_analysis.hpp"
#include "../display_drivers/display_parent.hpp"

class ModuleManager {

private:
    int nModules = 0;
    ModuleBase **REGISTERED_MODULES;
    ModuleBase *activeModule;
    int currentActiveModule = -1;

public:
    
    ModuleManager(DisplayParent *dispP) {
        *REGISTERED_MODULES = {
            new FrequencyAnalysisDisplayModule(dispP)
        };
    }

    ModuleBase ** getModules();

    int getNumberOfModules();

    int getCurrentActiveModule();

    ModuleBase * getActiveModule();

    void activateModule(int index);

    void deactivateModule(int index);

    void triggerModuleUpdate();

};

#endif

