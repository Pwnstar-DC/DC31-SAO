#ifndef BLE_SERVER_C
#define BLE_SERVER_C

#include "ble_server.hpp"

#define SERVICE_UUID "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

#include "bitmap"

void BLEServerModule::writeCharacteristic(BLECharacteristic *pCharacteristic)
{
  String value = pCharacteristic->getValue().c_str();

  if (value.length() > 0)
  {
    Serial.println("*********");
    for (int i = 0; i < value.length(); i++)
    {
      if(value == "ngtgyu")
      {
        for(int j=0; j<9; j++)
        {
          activeDisplay->drawBitmap(rickroll[j]);
          delay(50);
        }
        break;
      }
      else if (value == "hack the planet" || value == "Hack The Planet")
      {
        for(int k=0; k<24; k++)
        {
          activeDisplay->drawBitmap(hacktheplanet[k]);
        }
      }
      else
      {

        for(int l=0; l<59; l++)
        {
          activeDisplay->drawBitmap(lookaround_bm[l]);
        }

      }
        
    }
  }
}

void BLEServerModule::displaySplashScreen() {
  activeDisplay->clear();
  activeDisplay->writeTextToScreen("BLE Module", 0, 20);
  activeDisplay->flush();
}

void BLEServerModule::setup()
{
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
  // power saving features
  btStop();
  esp_bt_controller_disable();
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
