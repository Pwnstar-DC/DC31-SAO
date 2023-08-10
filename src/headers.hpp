#ifndef MAIN_H
#define MAIN_H

#include <Arduino.h>

void writeToSerial(String s);

void activeModuleCycle();

void registerPinActions();

void activeModuleModeCycle();

void toggleSleep();

void goToSleep();

void ledToggle();

void flashLeds(bool firstState, int firstStateTime, int secondStateTime);

void flashMessage();

void flashAll(char ch);

#endif 
