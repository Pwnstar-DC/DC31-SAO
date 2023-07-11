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

String ModuleManager::getActiveModuleString(ModuleBase * mod)
{
    return mod->getName(); 
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

void ModuleManager::nextModule() {
    // no reason to leave if only one module exists
    if(REGISTERED_MODULES.size() <= 1) {
        return;
    }
    
    // deactivate current module
    if(activeModule) {
        activeModule->deactivate();
        if(activeModule->activeDisplay) {
            activeModule->activeDisplay->flush();
        }
    }

    // get the next module logically from the list of modules
    bool foundCurrentModule = false;
    ModuleBase * nextModule = NULL;
    for(const auto& m : REGISTERED_MODULES) {
        if(foundCurrentModule) {
            // this is the next module to activate
            nextModule = m;
            break;
        }
        if(m->getName().equals(activeModule->getName())) {
            foundCurrentModule = true;
        }
    }

    if(nextModule) {
        activateModule(nextModule);
    }
    else{
        activateModule(REGISTERED_MODULES.at(0));
    }
}

#endif