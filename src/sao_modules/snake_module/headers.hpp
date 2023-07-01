
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
    static const int dimensions = 4; // needs to be an even division of screen width

    SnakeModuleBlock(int _locX, int _locY, int _xVector, int _yVector) {
        locX = _locX;
        locY = _locY;
        xVector = _xVector;
        yVector = _yVector;
    };
    ~SnakeModuleBlock() {};
};

class SnakeModule : public ModuleBase {
private:
    int chainLength = 16; // should be a multiple of block dimensions and a division of screen width
    int animationSpeedInd = 0;
    int nAnimationSpeeds = 3;
    int animationSpeeds[3] = {100, 50, 25};
    std::vector<std::shared_ptr<SnakeModuleBlock>> blocks;

public:

    SnakeModule(DisplayParent *display, String name) : ModuleBase(display, name) {};

    void setup();

    void displayUpdate();
    
    void logicUpdate();
    
    void teardown();

    void displaySplashScreen();

    void cycleMode();

};

#endif