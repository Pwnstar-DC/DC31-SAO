
#include "./display_drivers/ssd1306.hpp"
#include "./helpers/module_manager.hpp"
#include "headers.hpp"


#ifndef SERIAL_DEBUG
#define SERIAL_DEBUG 0
#endif


int i = 1;
int displayWidth = 0;
int displayHeight = 0;

ModuleManager *mm;

SSD1306 *display;

void setup() {
  Serial.setTxTimeoutMs(0);
  Serial.setTxBufferSize(5000);
  if(SERIAL_DEBUG){ 
    Serial.begin(115200);
    while (!Serial) {;}
    Serial.println();
    Serial.println();
  }
 
  writeToSerial("Starting...");
  writeToSerial("Initializing display...");
  display = new SSD1306;
  display->init();
  display->flush();
  displayWidth = display->getWidth();
  displayHeight = display->getHeight();
  display->writeTextToScreen("initializing...", displayWidth/2, displayHeight/2);
  writeToSerial("Finished initializing display...");
  writeToSerial("Initializing pins...");
  registerPinActions();
  writeToSerial("Finished initializing pins...");
  writeToSerial("Initializing Modules...");
  mm = new ModuleManager(display);
  writeToSerial("Activating initial module...");
  String initialModuleName = "snake_module";
  ModuleBase *mod = mm->getModuleByName(initialModuleName);
  if(mod) {
    mm->activateModule(mod);
  }
  else {
      display->clear();
      display->writeTextToScreen("Failed to find module: \n" + initialModuleName, 0, 0);
      display->flush();
  }
  writeToSerial("Setup complete");

}

void loop() {
  mm->triggerModuleUpdate();
}

void writeToSerial(String s) {
  if(!SERIAL_DEBUG) {
    return;
  }
  Serial.println(s);
  Serial.flush();
}

void registerPinActions() {

  #ifdef BOARD
    #if BOARD == seed_xiao_esp32c3
      // module mode cycle pin
      pinMode(D7, INPUT_PULLUP); // use input_pullup for activate on ground
      attachInterrupt(digitalPinToInterrupt(D7), activeModuleCycleModes, CHANGE);
      // rotate module pin
      pinMode(D8, INPUT_PULLUP); // use input_pullup for activate on ground
      attachInterrupt(digitalPinToInterrupt(D8), moduleCycle, CHANGE);
    #endif
  #endif
}

void activeModuleCycleModes() {
  #ifdef BOARD
    #if BOARD == seed_xiao_esp32c3
      // check that the D7 pin is currently HIGH, indicating that a
      // state change was triggered (button pressed) and the original
      // state was restored (button released)
      if(digitalRead(D7) != HIGH) {
        return;
      }
    #else
      return;  
    #endif
  #else
    return;
  #endif
  mm->getActiveModule()->cycleMode();
}

void moduleCycle() {
  #ifdef BOARD
    #if BOARD == seed_xiao_esp32c3
      // check that the D8 pin is currently HIGH, indicating that a
      // state change was triggered (button pressed) and the original
      // state was restored (button released)
      if(digitalRead(D8) != HIGH) {
        return;
      }
    #else
      return;  
    #endif
  #else
    return;
  #endif
  mm->nextModule();
}

