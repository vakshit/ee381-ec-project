#include <display.hpp>

Display::Display() { return; }

void Display::init() {
  // initialize the OLED object
  if (!this->display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ; // Don't proceed, loop forever
  }
  // Clear the buffer.
}

void Display::text(const String &text) {
  // Display Text
  this->display.clearDisplay();
  this->display.setTextSize(1);
  this->display.setTextColor(WHITE);
  this->display.setCursor(0, 28);
  this->display.println(text);
  this->display.display();
}