
#include "./display_drivers/ssd1306_128x32.hpp"
#include "./display_drivers/ssd1306_parent.hpp"

int i = 1;
int displayWidth = 0;
int displayHeight = 0;

SSD1306 *display;

void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println();

  display = new SSD1306_128x32;

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

  // modules?
  // register modules in array of modules
  // ModuleBase[] = [mod1, mod2, ...]
  // if we do modules, then menu? Buttons? Partition display? TBD
}
