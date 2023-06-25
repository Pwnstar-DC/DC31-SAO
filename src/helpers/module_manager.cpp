#ifndef MODULE_MANAGER_C
#define MODULE_MANAGER_C

#include "module_manager.hpp"
#include <vector>

std::vector<ModuleBase *> ModuleManager::getModules() {
    return REGISTERED_MODULES;
}

int ModuleManager::getNumberOfModules() {
    return REGISTERED_MODULES.size();
}

int ModuleManager::getCurrentActiveModule() {
    return currentActiveModule;
}

ModuleBase* ModuleManager::getActiveModule() {
    return activeModule;
}

void ModuleManager::activateModule(int index) {
    if(!(index >= 0 && index < REGISTERED_MODULES.size())) {
        return; // error case
    }
        if(index == currentActiveModule) {
        return;
    }

    ModuleBase * targetModule = REGISTERED_MODULES[index];    
    currentActiveModule = index;
    targetModule->setup();
    activeModule = targetModule;
}

void ModuleManager::deactivateModule(int index) {
    if(!(index >= 0 && index < REGISTERED_MODULES.size())) {
        return; // error case
    }

    ModuleBase * targetModule = REGISTERED_MODULES[index];
    // if a module is session-persistent, it should be suspended, not disassembled
    if(targetModule->getIsActive() && !targetModule->getIsSessionPersistent()) {
        targetModule->teardown();
    }
}

void ModuleManager::triggerModuleUpdate() {
    activeModule->waitForSync();
}

void ModuleManager::cycleMode() {
    activeModule->cycleMode();
}

#endif