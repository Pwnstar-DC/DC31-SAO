
#ifndef SNAKEMODULE_H
#define SNAKEMODULE_H

#include "../module_base.hpp"
#include <vector>
#include <bits/stdc++.h>

class SnakeModuleBlock {

public:
    int locX;
    int locY;
    int xVector = 0;
    int yVector = 0;
    int dimensions = 5;

    SnakeModuleBlock() {};
    ~SnakeModuleBlock() {};
};

class SnakeModule : public ModuleBase {
private:
    int chainLength = 5;
    std::vector<SnakeModuleBlock *> blocks;

public:

    SnakeModule(DisplayParent *display, String name) : ModuleBase(display, name) {};

    void setup();

    void displayUpdate();
    
    void logicUpdate();
    
    void teardown();

    void displaySplashScreen();

};

#endif