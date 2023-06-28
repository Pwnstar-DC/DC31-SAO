#ifndef SNAKEMODULE_C
#define SNAKEMODULE_C

#include "headers.hpp"

void SnakeModule::setup() {
    setLogicRefreshTime(100);
    setDisplayRefreshTime(100);

    // create all the blocks
    for(int i = 0; i < chainLength; i++) {
        blocks.push_back(new SnakeModuleBlock);
    }
};

void SnakeModule::displayUpdate() {
    activeDisplay->clear();
    // display the blocks
    for(const auto& bl : blocks) {
        // draw block outline with origin top left corner at locXxlocY
        activeDisplay->drawRect(
            bl->locX,
            bl->locY,
            bl->dimensions,
            bl->dimensions
        );
    }
    activeDisplay->flush();
};

void SnakeModule::displaySplashScreen() {
    activeDisplay->writeTextToScreen("Snake module", 0, 0);
    activeDisplay->flush();
}

void SnakeModule::logicUpdate()  {
    for(const auto& bl: blocks) {
        bl->locX += bl->dimensions * bl->xVector;
        bl->locY += bl->dimensions * bl->yVector;

        // check if blocks go out of bounding area (screen max width or height - dimensions)
        // based on current vector

        // there will never be a case where an xVector will be set at the same time as a yVector4
        // (at least for now)
        if(bl->xVector != 0) {
            if(bl->locX >= activeDisplay->getWidth()) {
                bl->locX -= bl->dimensions;
                bl->xVector = 0;
                bl->yVector = 1;
            }
            if(bl->locX <= activeDisplay->getWidth()) {
                bl->locX += bl->dimensions;
                bl->xVector = 0;
                bl->yVector = -1;
            }

             if(bl->locY >= activeDisplay->getHeight()) {
                bl->locY -= bl->dimensions;
                bl->yVector = 0;
                bl->xVector = -1;
            }
            if(bl->locY <= activeDisplay->getHeight()) {
                bl->locY += bl->dimensions;
                bl->yVector = 0;
                bl->xVector = 1;
            }
        }
    }
    return;
};

void SnakeModule::teardown() {
    // no need to free the snake module blocks since they are smart pointers
}

#endif