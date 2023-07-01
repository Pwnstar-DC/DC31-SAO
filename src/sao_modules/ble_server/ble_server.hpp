#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include "../../sao_modules/module_base.hpp"
#include "../../display_drivers/display_parent.hpp"
#include "../../helpers/scrolling_buffer.hpp"
#include "../..//display_drivers/ssd1306.hpp"

class BLEServerModule : public ModuleBase {

public:
    BLEServerModule(DisplayParent *display, String name) : ModuleBase(display, name) {};

    void setup();

    void printBuffer();

    void writeCharacteristic(BLECharacteristic *pCharacteristic);

};
