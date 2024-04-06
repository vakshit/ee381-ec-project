#pragma once

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SPI.h>
#include <Wire.h>

class Display {
private:
  const int SCREEN_WIDTH = 128; // OLED display width, in pixels
  const int SCREEN_HEIGHT = 64; // OLED display height, in pixels

  // Declaration for SSD1306 display connected using I2C
  const int OLED_RESET = -1; // Reset pin
  const int SCREEN_ADDRESS = 0x3C;
  Adafruit_SSD1306 display =
      Adafruit_SSD1306(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

public:
  Display();
  void init();
  void text(const String &text);
};

// void setup() {

//   // Display Inverted Text
//   display.setTextColor(BLACK, WHITE); // 'inverted' text
//   display.setCursor(0, 28);
//   display.println("Hello world!");
//   display.display();
//   delay(2000);
//   display.clearDisplay();

//   // Changing Font Size
//   display.setTextColor(WHITE);
//   display.setCursor(0, 24);
//   display.setTextSize(2);
//   display.println("Hello!");
//   display.display();
//   delay(2000);
//   display.clearDisplay();

//   // Display Numbers
//   display.setTextSize(1);
//   display.setCursor(0, 28);
//   display.println(123456789);
//   display.display();
//   delay(2000);
//   display.clearDisplay();

//   // Specifying Base For Numbers
//   display.setCursor(0, 28);
//   display.print("0x");
//   display.print(0xFF, HEX);
//   display.print("(HEX) = ");
//   display.print(0xFF, DEC);
//   display.println("(DEC)");
//   display.display();
//   delay(2000);
//   display.clearDisplay();

//   // Display ASCII Characters
//   display.setCursor(0, 24);
//   display.setTextSize(2);
//   display.write(3);
//   display.display();
//   delay(2000);
//   display.clearDisplay();

//   // Scroll full screen
//   display.setCursor(0, 0);
//   display.setTextSize(1);
//   display.println("Full");
//   display.println("screen");
//   display.println("scrolling!");
//   display.display();
//   display.startscrollright(0x00, 0x07);
//   delay(2000);
//   display.stopscroll();
//   delay(1000);
//   display.startscrollleft(0x00, 0x07);
//   delay(2000);
//   display.stopscroll();
//   delay(1000);
//   display.startscrolldiagright(0x00, 0x07);
//   delay(2000);
//   display.startscrolldiagleft(0x00, 0x07);
//   delay(2000);
//   display.stopscroll();
//   display.clearDisplay();

//   // Scroll part of the screen
//   display.setCursor(0, 0);
//   display.setTextSize(1);
//   display.println("Scroll");
//   display.println("some part");
//   display.println("of the screen.");
//   display.display();
//   display.startscrollright(0x00, 0x00);
// }
