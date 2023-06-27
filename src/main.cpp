
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
  if(SERIAL_DEBUG){ 
    Serial.begin(115200);
    Serial.setTxTimeoutMs(0);
    while (!Serial) {;}
    Serial.setTxBufferSize(5000);
    Serial.println();
    Serial.println();
  }
 

  writeToSerial("Starting...");
  writeToSerial("Initializing display...");
  display = new SSD1306;
  display->init();
  display->flush();
  displayWidth = display->getRelativeMaxWidth();
  displayHeight = display->getRelativeMaxHeight();
  display->writeTextToScreen("initializing...", displayWidth/2, displayHeight/2);
  writeToSerial("Finished initializing display...");
  writeToSerial("Initializing Modules...");
  mm = new ModuleManager(display);
  writeToSerial("Activating first  module...");
  mm->activateModule(0);
  writeToSerial("Setup complete");
}

void loop() {
  mm->triggerModuleUpdate();
  delay(100);
}

void writeToSerial(String s) {
  if(!SERIAL_DEBUG) {
    return;
  }
  Serial.println(s);
  Serial.flush();
}
