#ifndef BLE_SERVER_C
#define BLE_SERVER_C

#include "ble_server.hpp"

#define SERVICE_UUID "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

#include "bitmap"

const unsigned char** gif_bitmap;
int counter = 0;
int MAX_COUNT = 0;


void BLEServerModule::displaySplashScreen() {
  activeDisplay->clear();
  activeDisplay->writeTextToScreen("BLE Module", 0, 20);
  activeDisplay->flush();
}

void BLEServerModule::setup()
{
  setCpuFrequencyMhz(80); // minimum required to operate this module
  delay(25);
  gif_bitmap = lookaround_bm;
  MAX_COUNT = 59;
  setLogicRefreshTime(100);
  setDisplayRefreshTime(50);
  
  activeDisplay->flush();

  Serial.println("Reached BLE Setup");
  BLEDevice::init("ESP32-BLE-Server");
  pServer = BLEDevice::createServer();

  pService = pServer->createService(SERVICE_UUID);

   pCharacteristic = pService->createCharacteristic(
    CHARACTERISTIC_UUID,
    BLECharacteristic::PROPERTY_READ |
    BLECharacteristic::PROPERTY_WRITE
  );

  pCharacteristic->setValue("Hello World");
  pService->start();

	pAdvertising = pServer->getAdvertising();
	pAdvertising->start();

}

void BLEServerModule::teardown() {
  pService->stop();
  pAdvertising->stop();
  // power saving features
  btStop();
  esp_bt_controller_disable();
  setCpuFrequencyMhz(getXtalFrequencyMhz()); // reset to minimum required to operate controller
}

void BLEServerModule::logicUpdate(int64_t lastMetaLogicUpdate) {
  String compare;
  String valtostring;
  valtostring = pCharacteristic->getValue().c_str();

  if(valtostring == "ngtgyu")
  {
    counter = 0;
    MAX_COUNT = 9;
    gif_bitmap = rickroll;
  }
  if(valtostring == "hack the planet")
  {
    counter = 0;
    MAX_COUNT = 24;
    gif_bitmap = hacktheplanet;
  }

  pCharacteristic->setValue("Hello World");
}

void BLEServerModule::displayUpdate(int64_t lastMetaDisplayUpdate) {
  if(counter == MAX_COUNT) counter = 0;
  activeDisplay->drawBitmap(gif_bitmap[counter]);
  counter++;
  activeDisplay->flush();
}


#endif
