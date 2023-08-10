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
  if(SERIAL_DEBUG) {
    Serial.println("Reached BLE Setup");
  }
  setCpuFrequencyMhz(80); // minimum required to operate this module
  delay(25);
  gif_bitmap = lookaround_bm;
  MAX_COUNT = 59;
  setLogicRefreshTime(100);
  setDisplayRefreshTime(50);
  
  activeDisplay->flush();

  BLEDevice::init("ESP32-BLE-Server");
  BLEDevice::setPower(ESP_PWR_LVL_N18); // ref esp_bt., corresponds to power level -18
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
  if(SERIAL_DEBUG) {
    Serial.println("BLE setup done");
  }
}

void BLEServerModule::teardown() {
  if(SERIAL_DEBUG) {
    Serial.println("Tearing down BLE module");
  }
  pService->stop();
  pAdvertising->stop();
  // power saving features
  setCpuFrequencyMhz(getXtalFrequencyMhz()); // reset to minimum required to operate controller
  if(SERIAL_DEBUG) {
    Serial.println("BLE module teardown complete");
  }
}

void BLEServerModule::logicUpdate(uint32_t lastMetaLogicUpdate) {
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

void BLEServerModule::displayUpdate(uint32_t lastMetaDisplayUpdate) {
  if(counter == MAX_COUNT) counter = 0;
  activeDisplay->drawBitmap(gif_bitmap[counter]);
  counter++;
  activeDisplay->flush();
}


#endif
