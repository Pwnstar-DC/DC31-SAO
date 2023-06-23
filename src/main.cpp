
#include "./display_drivers/ssd1306_128x64.hpp"
#include "./display_drivers/ssd1306_parent.hpp"
#include "./helpers/module_manager.hpp"

int i = 1;
int displayWidth = 0;
int displayHeight = 0;

SSD1306 *display;

void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println();

  Serial.println("Starting...");

  display = new SSD1306_128x64;

  display->init();
  displayWidth = display->getRelativeMaxWidth();
  displayHeight = display->getRelativeMaxHeight();

  

}

void loop() {
  display->writeTextToScreen(String(i), displayWidth / 2, displayHeight / 2);
  display->flush();
  Serial.println(String(i));
  delay(100);
  i += 1;

 
}
