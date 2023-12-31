#ifndef SNAKEMODULE_C
#define SNAKEMODULE_C

#include "headers.hpp"

void SnakeModule::setup() {
    setLogicRefreshTime(25);
    setDisplayRefreshTime(25);


    int blockDims = 4; //SnakeModuleBlock.dimensions;
    // create all the blocks
    for(int i = 0; i < chainLength; i++) {
        blocks.push_back(
            std::shared_ptr<SnakeModuleBlock>(new SnakeModuleBlock(
                i*blockDims,
                0,
                1,
                0
            ))
        );

        dualingBlocks.push_back(
            std::shared_ptr<SnakeModuleBlock>(new SnakeModuleBlock(
                activeDisplay->getWidth() + (-i*blockDims),
                activeDisplay->getHeight() - blockDims,
                1,
                0
            ))
        );
    }

    if(SERIAL_DEBUG) {
        Serial.println("SM - Created a chain with n blocks: " + String(blocks.size()));
        Serial.flush();
    }
};

void SnakeModule::displayUpdate(uint32_t lastMetaDisplayUpdate) {
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

    if(dualingSnakes) {
        for(const auto& bl : dualingBlocks) {
            // draw block outline with origin top left corner at locXxlocY
            activeDisplay->drawRect(
                bl->locX,
                bl->locY,
                bl->dimensions,
                bl->dimensions
            );
        }
    }
    activeDisplay->flush();
};

void SnakeModule::displaySplashScreen() {
    activeDisplay->writeTextToScreen("Snake module", 0, 0);
    activeDisplay->flush();
}

void SnakeModule::logicUpdate(uint32_t lastMetaLogicUpdate)  {
    for(const auto& bl: blocks) {
        animateBlock(bl);
    }
    for(const auto& bl: dualingBlocks) {
        animateBlock(bl);
    };
};

void SnakeModule::animateBlock(std::shared_ptr<SnakeModuleBlock> bl) {
    bl->locX += (bl->dimensions * bl->xVector);
    bl->locY += (bl->dimensions * bl->yVector);

    // check if blocks go out of bounding area (screen max width or height - dimensions)
    // based on current vector

    // there will never be a case where an xVector will be set at the same time as a yVector4
    // (at least for now)
    if(bl->locX >= activeDisplay->getWidth()) {
        bl->locX -= bl->dimensions;
        bl->xVector = 0;
        bl->yVector = 1;
    }
    else if(bl->locX <= 0) {
        bl->locX += bl->dimensions;
        bl->xVector = 0;
        bl->yVector = -1;
    }
    else if(bl->locY >= activeDisplay->getHeight()) {
        bl->locY -= bl->dimensions;
        bl->yVector = 0;
        bl->xVector = -1;
    }
    else if(bl->locY <= 0) {
        bl->locY += bl->dimensions;
        bl->yVector = 0;
        bl->xVector = 1;
    }
}

void SnakeModule::cycleMode() {
    setLogicRefreshTime(animationSpeeds[animationSpeedInd]);
    setDisplayRefreshTime(animationSpeeds[animationSpeedInd]);

    animationSpeedInd = (animationSpeedInd + 1) % nAnimationSpeeds;
    // dual snakes after a full rotation of speeds
    if(animationSpeedInd == 0) {
        dualingSnakes = !dualingSnakes;
    }
}

void SnakeModule::teardown() {
    // no need to free the snake module blocks since they are smart pointers
    blocks.clear();
    dualingBlocks.clear();
}

#endif