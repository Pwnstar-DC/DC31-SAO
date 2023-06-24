
#include "./display_drivers/ssd1306.hpp"
#include "./helpers/module_manager.hpp"
#include "headers.hpp"

int i = 1;
int displayWidth = 0;
int displayHeight = 0;
ModuleManager *mm;

SSD1306 *display;

void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println();
  //delay(1000);

  writeToSerial("Starting...");
  writeToSerial("Initializing display...");
  display = new SSD1306;
  delay(500);
  display->init();
  displayWidth = display->getRelativeMaxWidth();
  displayHeight = display->getRelativeMaxHeight();
  writeToSerial("Finished initializing display...");
  writeToSerial("Initializing Modules...");
  mm = new ModuleManager(display);
  delay(500);
  writeToSerial("Activating first  module...");
  mm->activateModule(0);
  writeToSerial("Setup complete");
}

void loop() {
  mm->triggerModuleUpdate();
}

void writeToSerial(String s) {
  Serial.println(s);
  Serial.flush();
}
