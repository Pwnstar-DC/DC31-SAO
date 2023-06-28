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

ModuleBase* ModuleManager::getActiveModule() {
    return activeModule;
}

void ModuleManager::activateModule(int index) {
    if(!(index >= 0 && index < REGISTERED_MODULES.size())) {
        return; // error case
    }

    ModuleBase * targetModule = REGISTERED_MODULES[index];    
    activateModule(targetModule);
}

void ModuleManager::activateModule(ModuleBase * mod) {
    mod->displaySplashScreen();
    mod->setup();
    activeModule = mod;
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
    if(activeModule) {
        activeModule->waitForSync();
    }
}

void ModuleManager::cycleMode() {
    activeModule->cycleMode();
}

ModuleBase * ModuleManager::getModuleByName(String name) {
    for(const auto& mod : REGISTERED_MODULES) {
        if(mod->getName().equals(name)) {
            return mod;
        }
    }
    return NULL;
}

#endif