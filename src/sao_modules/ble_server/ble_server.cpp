#ifndef BLE_SERVER_C
#define BLE_SERVER_C

#include "ble_server.hpp"

#define SERVICE_UUID "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

#include "bitmap"

void BLEServerModule::printBuffer() {
  const char* test[] = {
    "Hack",
    "The",
    "Planet"};

    for (uint8_t i = 0; i < 11; i++) {
      // Print to the screen
      //activeDisplay->writeTest("It Worked!", 0, 0);
      // Draw it to the internal screen buffer
      //display.drawLogBuffer(0, 0);
      // Display it on the screen
      //display.display();
      delay(500);
  }
}

void BLEServerModule::writeCharacteristic(BLECharacteristic *pCharacteristic)
{
  String value = pCharacteristic->getValue().c_str();

  if (value.length() > 0)
  {
    Serial.println("*********");
    for (int i = 0; i < value.length(); i++)
    {
      if (value == "hack the planet" || value == "Hack the Planet" || value == "Hack The Planet")
      {
        activeDisplay->drawBitmap(bm1);
        activeDisplay->drawBitmap(bm2);
        activeDisplay->drawBitmap(bm3);
        activeDisplay->drawBitmap(bm4);
        activeDisplay->drawBitmap(bm5);
        activeDisplay->drawBitmap(bm6);
        activeDisplay->drawBitmap(bm7);
        activeDisplay->drawBitmap(bm8);
        activeDisplay->drawBitmap(bm9);
        //delay(1000);
        break;
      }
        
    }

    Serial.println();
    Serial.println("*********");
  }
}

void BLEServerModule::displaySplashScreen() {
  activeDisplay->clear();
  activeDisplay->writeTextToScreen("BLE Module", 0, 20);
  activeDisplay->flush();
}

void BLEServerModule::setup()
{
  activeDisplay->drawBitmap(bm1);
  activeDisplay->drawBitmap(bm2);
  activeDisplay->drawBitmap(bm3);
  activeDisplay->drawBitmap(bm4);
  activeDisplay->drawBitmap(bm5);
  activeDisplay->drawBitmap(bm6);
  activeDisplay->drawBitmap(bm7);
  activeDisplay->drawBitmap(bm8);
  activeDisplay->drawBitmap(bm9);
  setLogicRefreshTime(100);
  setDisplayRefreshTime(200);
  
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
}

void BLEServerModule::logicUpdate() {
  String compare;
  String valtostring;
  valtostring = pCharacteristic->getValue().c_str();
    if(compare != valtostring)
    {
      Serial.println("Characteristic Written");
      writeCharacteristic(pCharacteristic);
    }
    compare = valtostring;
}

void BLEServerModule::displayUpdate() {
  activeDisplay->flush();
}

#endif
