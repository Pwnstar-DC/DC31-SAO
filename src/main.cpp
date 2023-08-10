
#include "./display_drivers/ssd1306.hpp"
#include "./helpers/module_manager.hpp"
#include "headers.hpp"
#include <esp_bt.h>
#include <esp_wifi.h>


#ifndef SERIAL_DEBUG
#define SERIAL_DEBUG 0
#endif


//int i = 1;
int displayWidth = 0;
int displayHeight = 0;

bool shiftModules = false;
bool shiftModes = false;
bool checkSleep = false;
bool checkLed = false;
int ledState = 2;
uint32_t boot_time;

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
  display->writeTextToScreen("initializing...", 0, displayHeight/2);
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
  esp_wifi_stop();
  if(mm->getActiveModuleString(mod) != "ble_server_module")
  {
    esp_bt_controller_disable();
  }
  display->setBrightness(40);
  setCpuFrequencyMhz(getXtalFrequencyMhz());
  boot_time = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
  writeToSerial("Setup complete");
}

void loop() {

  if(shiftModes || shiftModules || checkSleep || checkLed) {
    if(SERIAL_DEBUG) {
        Serial.println("Main: Interrupt");
        Serial.flush();
    }
    uint32_t now = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();

    // protect the button triggers for 1 second after boot to prevent startup-related rising triggers
    if(now + 1000 > boot_time) {
        if(shiftModes) {
        mm->getActiveModule()->cycleMode();
      }
      if(shiftModules) {
        mm->nextModule();
      }
      if(checkSleep) {
        goToSleep();
      }
      if(checkLed) {
        ledState += 1;
        if(ledState > 2) {
          ledState = 0;
        }
        #ifdef BOARD
          #if BOARD == seed_xiao_esp32c3
              ledcWrite(0, 128 * ledState);
              ledcWrite(1, 128 * ledState);
              ledcWrite(2, 128 * ledState);
          #endif
        #endif
      }
    }

    shiftModes = false;
    shiftModules = false;
    checkLed = false;
    checkSleep = false;

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
      // toggle led
      pinMode(D7, INPUT_PULLUP); // use input_pullup for activate on ground
      attachInterrupt(digitalPinToInterrupt(D7), ledToggle, RISING); // rising to activate when button released

      ledcSetup(0, 10000, 8);
      ledcSetup(1, 10000, 8);
      ledcSetup(2, 10000, 8);
      ledcAttachPin(A0, 0);
      ledcAttachPin(A1, 1);
      ledcAttachPin(A2, 2);
      ledcWrite(0, 128 * ledState);
      ledcWrite(1, 128 * ledState);
      ledcWrite(2, 128 * ledState);

      // rotate module pin
      pinMode(D8, INPUT_PULLUP); // use input_pullup for activate on ground
      attachInterrupt(digitalPinToInterrupt(D8), activeModuleCycle, RISING); // rising to activate when button released
      // module mode cycle pin
      pinMode(D9, INPUT_PULLUP); // use input_pullup for activate on ground
      attachInterrupt(digitalPinToInterrupt(D9), activeModuleModeCycle, RISING); // rising to activate when button released
      
    #endif
  #endif
}

void activeModuleModeCycle() {
  shiftModes = true;
}

void activeModuleCycle() {
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

void flashLeds(bool firstState, int firstStateTime, int secondStateTime) {
  bool currentState = firstState;
  // set led to first state
  ledcWrite(0, 256 * currentState);
  ledcWrite(1, 256 * currentState);
  ledcWrite(2, 256 * currentState);
  delay(firstStateTime);
  // set to second state
  currentState = !currentState;
  ledcWrite(0, 256 * currentState);
  ledcWrite(1, 256 * currentState);
  ledcWrite(2, 256 * currentState);
  delay(secondStateTime);
  /// revert state
  ledcWrite(0, 128 * currentState);
  ledcWrite(1, 128 * currentState);
  ledcWrite(2, 128 * currentState);
}

