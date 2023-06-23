
#include "./display_drivers/ssd1306_128x64.hpp"
#include "./display_drivers/ssd1306_128x32.hpp"
#include "./display_drivers/ssd1306_parent.hpp"
#include "./helpers/module_manager.hpp"

int i = 1;
int displayWidth = 0;
int displayHeight = 0;
ModuleManager *mm;

SSD1306 *display;

void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println();

  Serial.println("Starting...");

  #ifndef SCREEN_SIZE
  display = new SSD1306_128x64;
  #elif SCREEN_SIZE == 32
  display = new SSD1306_128x32;
  #elif SCREEN_SIZE == 64
  display = new SSD1306_128x64
  #else
  display = new SSD1306_128x64;
  #endif

  display->init();
  displayWidth = display->getRelativeMaxWidth();
  displayHeight = display->getRelativeMaxHeight();

  mm = new ModuleManager(display);
  mm->activateModule(0);
}

void loop() {
  
  mm->getActiveModule()->update();
  delay(100);
}
