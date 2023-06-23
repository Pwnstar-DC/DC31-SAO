#ifndef FREQ_ANALYSIS_C
#define FREQ_ANALYSIS_C

#include "freq_analysis.hpp"

byte data[128];

void draw(SSD1306 *display)
{
  int HighestPeak;
  int Height;

  //draw X axis notches:
  for(byte x = 0; x < 128; x+=10)
  {
    //draw notches every 10 ticks to allow identifying the range of frequencies
    //u8g2.drawVLine(x,62,2);
    display->drawVLine(x,62,2);
  }

}

void freq_main(SSD1306 *display)
{

  display->write_test("Scan Start", 0, 0);
  display->flush();

  int num;
  while(num = WiFi.scanNetworks())
  {
    draw(display);
  
    // WiFi.scanNetworks will return the number of networks found
    //int num = WiFi.scanNetworks();

    if (num == 0) {
        display->write_test("No Networks Found", 0, 0);
        display->flush();
    } else {
      String networks = String(num) + " Networks Found";
      display->write_test(networks, 0, 0);

      for (int i = 0; i < num; ++i) {

        Serial.println(WiFi.RSSI(i));
        //display.println(")");
        //display.println((WiFi.encryptionType(i) == WIFI_AUTH_OPEN)?" ":"*");
        data[i] = WiFi.RSSI(i);
        //delay(10);
      }

      int j = 100;
      for(int i=0; i < num; i++)
      {
        if(i=0)
        {
          display->write_test(String(WiFi.RSSI(i)), 0, 30);
        }
        display->write_test(String(WiFi.RSSI(i)), j, 30);
        j += 100;
      }

      display->flush();

      //display->flush();
      //draw();
    }
  }
}




void freq_setup(SSD1306 *display) {

  //Serial.begin(115200);

  // Set WiFi to station mode and disconnect from an AP if it was previously connected
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  Serial.println("Setup done");



  freq_main(display);

}



#endif
