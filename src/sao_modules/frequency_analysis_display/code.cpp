#include "WiFi.h"
#include <Wire.h> 
#include "SSD1306Wire.h" 
#include <SPI.h>
#include "src/cc2500_REG.h"

SSD1306Wire display(0x3c, 5, 4); 

byte data[128];

void draw(void)
{
  int HighestPeak;
  int Height;
  for(byte x = 0; x < 128; x++)
  {

    Height = data[x];

    if (Height > 60) Height = 60;
    
    if (Height > 0) {
      //u8g2.drawVLine(x,50-Height,50);
      //u8g2.drawVLine(x,60-Height,Height);

      //ssd1306 draw vertical
      //display.drawVerticalLine(x,60-Height,Height);
      display.drawVerticalLine(x, 60-Height, Height);
      display.display();
      delay(10);
      //display.println("Testing");setLog
      //Serial.printf("Test");

    }
  }

  //draw X axis notches:
  for(byte x = 0; x < 128; x+=10)
  {
    //draw notches every 10 ticks to allow identifying the range of frequencies
    //u8g2.drawVLine(x,62,2);

    //ssd1306 draw vertical
    display.drawVerticalLine(x,62,2);
  }

    //Display the highest peak observed recently:
    //u8g2.setFont(u8g2_font_ncenB08_tr);
    //u8g2.setCursor(100, 12);
    //u8g2.print(HighestPeak);

    //ssd1306 print highest peak
    display.setLogBuffer(5, 30);
    //display.clear();
    //display.println("Test");
    display.setFont(ArialMT_Plain_10);
    display.print("Highest Peak: ");
    display.println(HighestPeak);
    display.print("Average Peak: ");
    //display.println(AvgPeak);
    // Draw it to the internal screen buffer
    display.drawLogBuffer(0, 0);
    // Display it on the screen
    display.display();
    //delay(1000);
    //print the index so I can keep track of how long an interval the peak represents:
//    u8g2.setCursor(100, 42);
//    u8g2.print(peakindex);
    

}


void setup() {
  display.init();

  // display.flipScreenVertically();
  display.setContrast(255);

  Serial.begin(115200);

  // Set WiFi to station mode and disconnect from an AP if it was previously connected
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  Serial.println("Setup done");

}

void loop()
{

  display.clear();
  display.setLogBuffer(30, 30);
  display.println("scan start");

  // WiFi.scanNetworks will return the number of networks found
  int n = WiFi.scanNetworks();
  display.println("scan done");
  if (n == 0) {
      display.println("no networks found");
  } else {
    display.print(n);
    display.println(" networks found");
    for (int i = 0; i < n; ++i) {
      // Print SSID and RSSI for each network found
      display.println(i + 1);
      display.println(": ");
      display.println(WiFi.SSID(i));
      display.println(" (");
      Serial.println(WiFi.RSSI(i));
      display.println(")");
      display.println((WiFi.encryptionType(i) == WIFI_AUTH_OPEN)?" ":"*");
      data[i] = WiFi.RSSI(i);
      delay(10);
    }
    //display.drawLogBuffer(0, 0);
    //display.display();
    draw();

/*
    display.setLogBuffer(5, 30);
    display.clear();
    //display.println("Test");
    display.setFont(ArialMT_Plain_10);
    display.print("Highest Peak: ");
    display.println(HighestPeak);
    display.print("Average Peak: ");
    display.println(AvgPeak);
    // Draw it to the internal screen buffer
    display.drawLogBuffer(0, 0);
    // Display it on the screen
    display.display();
*/
    //draw();
  }

}
