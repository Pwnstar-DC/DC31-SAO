#ifndef MAIN_H
#define MAIN_H

#include <Arduino.h>

void writeToSerial(String s);

void activeModuleCycleModes();

void registerPinActions();

void moduleCycle();

void toggleSleep();

void goToSleep();

void ledToggle();

#endif 