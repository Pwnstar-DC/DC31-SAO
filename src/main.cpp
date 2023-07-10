
#include "./display_drivers/ssd1306.hpp"
#include "./helpers/module_manager.hpp"
#include "headers.hpp"
#include <esp_bt.h>
#include <esp_wifi.h>


#ifndef SERIAL_DEBUG
#define SERIAL_DEBUG 0
#endif


int i = 1;
int displayWidth = 0;
int displayHeight = 0;

bool shiftModules = false;
bool shiftModes = false;
bool checkSleep = false;
bool checkLed = true;
bool ledOn = true;


ModuleManager *mm;

SSD1306 *display;

void setup() {
  //Serial.setRxTimeout(0);
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
  display->writeTextToScreen("initializing..." + String(getXtalFrequencyMhz())+"mhz", 0, displayHeight/2);
  display->flush();
  delay(2000);
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


  // do power saving features
  setCpuFrequencyMhz(getXtalFrequencyMhz()); // set minimum possible frequency per Xtal oscilator
  esp_wifi_stop();
  esp_bt_controller_disable();
  display->setBrightness(50);
  if(getCpuFrequencyMhz() > getXtalFrequencyMhz()) {
    setCpuFrequencyMhz(80);
  }

  writeToSerial("Setup complete");

}

void loop() {

  if(shiftModes) {
    mm->getActiveModule()->cycleMode();
    shiftModes = false;
  }
  if(shiftModules) {
    mm->nextModule();
    shiftModules = false;
  }
  if(checkSleep) {
    goToSleep();
    checkSleep = false;
  }
  if(checkLed) {
    checkLed = false;
    ledOn = !ledOn;
    #ifdef BOARD
      #if BOARD == seed_xiao_esp32c3
          digitalWrite(D2, int(ledOn));
      #endif
    #endif
  }
  mm->triggerModuleUpdate();
}

void writeToSerial(String s) {
  if(!SERIAL_DEBUG) {
    return;
  }
  Serial.println(s);
  Serial.flush();
}

//NOTE: you can NOT use serial actions in interrupts
// Per Documented standards, it is recommended to set flags
// in the interrupts, not do significant logic
void registerPinActions() {
  #ifdef BOARD
    #if BOARD == seed_xiao_esp32c3
      // module mode cycle pin
      pinMode(D0, INPUT_PULLUP); // use input_pullup for activate on ground
      attachInterrupt(digitalPinToInterrupt(D0), activeModuleCycleModes, RISING); // rising to activate when button released
      // rotate module pin
      pinMode(D10, INPUT_PULLUP); // use input_pullup for activate on ground
      attachInterrupt(digitalPinToInterrupt(D10), moduleCycle, RISING); // rising to activate when button released
      // toggle led
      pinMode(D8, INPUT_PULLUP); // use input_pullup for activate on ground
      attachInterrupt(digitalPinToInterrupt(D8), ledToggle, RISING); // rising to activate when button released
      pinMode(D2, OUTPUT);
      digitalWrite(D2, HIGH);
    #endif
  #endif
}

void activeModuleCycleModes() {
  shiftModes = true;
}

void moduleCycle() {
  shiftModules = true;
}

void toggleSleep() {
   checkSleep = true;
}

void ledToggle() {
  checkLed = true;
}

void goToSleep() {
  display->setDisplayOff();
  esp_deep_sleep_start();
  display->setDisplayOn();
}

