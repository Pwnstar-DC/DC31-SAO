#ifndef BLE_SERVER_C
#define BLE_SERVER_C

#include "ble_server.hpp"

#define SERVICE_UUID "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

SSD1306 *bleDisplay;

void BLEServerModule::printBuffer() {
  const char* test[] = {
    "Hack",
    "The",
    "Planet"};

    for (uint8_t i = 0; i < 11; i++) {
      activeDisplay->flush();
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
        activeDisplay->writeTest("Written to characteristic", 0, 0);
        Serial.println("Written to characteristic");
        delay(1000);
        break;
      }
        
    }

    Serial.println();
    Serial.println("*********");
  }
}

void BLEServerModule::setup()
{
  
  activeDisplay->flush();

  Serial.println("Reached BLE Setup");

  BLEDevice::init("ESP32-BLE-Server");
  BLEServer *pServer = BLEDevice::createServer();

  BLEService *pService = pServer->createService(SERVICE_UUID);

  BLECharacteristic *pCharacteristic = pService->createCharacteristic(
    CHARACTERISTIC_UUID,
    BLECharacteristic::PROPERTY_READ |
    BLECharacteristic::PROPERTY_WRITE
  );

  //pCharacteristic->setCallbacks(new MyCallbacks());

  pCharacteristic->setValue("Hello World");
  pService->start();

	BLEAdvertising *pAdvertising = pServer->getAdvertising();
	pAdvertising->start();

  String compare;
  String valtostring;
  
  while(1)
  {
    valtostring = pCharacteristic->getValue().c_str();
    if(compare != valtostring)
    {
      Serial.println("Characteristic Written");
      writeCharacteristic(pCharacteristic);
    }
    compare = valtostring;
  }

}

#endif