/*******************************************************************
    A touch screen test for the ESP32 Cheap Yellow Display.

    https://github.com/witnessmenow/ESP32-Cheap-Yellow-Display

    If you find what I do useful and would like to support me,
    please consider becoming a sponsor on Github
    https://github.com/sponsors/witnessmenow/

    Written by Brian Lough
    YouTube: https://www.youtube.com/brianlough
    Twitter: https://twitter.com/witnessmenow
 *******************************************************************/

// Make sure to copy the UserSetup.h file into the library as
// per the Github Instructions. The pins are defined in there.

// ----------------------------
// Standard Libraries
// ----------------------------

#include <SPI.h>

// ----------------------------
// Additional Libraries - each one of these will need to be installed.
// ----------------------------

#include <XPT2046_Touchscreen.h>
// A library for interfacing with the touch screen
//
// Can be installed from the library manager (Search for "XPT2046")
// https://github.com/PaulStoffregen/XPT2046_Touchscreen

#include <TFT_eSPI.h>
// A library for interfacing with LCD displays
//
// Can be installed from the library manager (Search for "TFT_eSPI")
// https://github.com/Bodmer/TFT_eSPI

// ----------------------------
// Touch Screen pins
// ----------------------------

// The CYD touch uses some non default
// SPI pins

#define XPT2046_IRQ 36
#define XPT2046_MOSI 32
#define XPT2046_MISO 39
#define XPT2046_CLK 25
#define XPT2046_CS 33

// ----------------------------

SPIClass mySpi = SPIClass(VSPI);
XPT2046_Touchscreen ts(XPT2046_CS, XPT2046_IRQ);

TFT_eSPI tft = TFT_eSPI();

int rectX = 60;
int rectY = 80;
int rectW = 60;
int rectH = 40;

void setup()
{
  Serial.begin(115200);

  // Start the SPI for the touch screen and init the TS library
  mySpi.begin(XPT2046_CLK, XPT2046_MISO, XPT2046_MOSI, XPT2046_CS);
  ts.begin(mySpi);
  ts.setRotation(1);

  // Start the tft display and set it to black
  tft.init();
  tft.setRotation(1); // This is the display in landscape

  // Clear the screen before writing to it
  tft.fillScreen(TFT_BLACK);

  int x = 320 / 2; // center of display
  int y = 100;
  int fontSize = 2;
  // tft.drawCentreString("Touch Screen to Start", x, y, fontSize);
}

void drawRectangle(int x, int y) {
  tft.fillRect(x, y, rectW, rectH, TFT_RED);
}

void moveRectangle(String direction) {
  // Erase old rectangle
  tft.fillRect(rectX, rectY, rectW, rectH, TFT_BLACK);

  // Update position
  if (direction == "UP") {
    for(int i = 0; i < 10; i++) {
      tft.fillRect(rectX, rectY, rectW, rectH, TFT_BLACK);
      rectY -= 1;
      drawRectangle(rectX, rectY);
    }
  }
  else if (direction == "DOWN") rectY += 10;
  else if (direction == "LEFT") rectX -= 10;
  else if (direction == "RIGHT") rectX += 10;

  // Boundary check
  if (rectX < 0) rectX = 0;
  if (rectY < 0) rectY = 0;
  if (rectX + rectW > tft.width()) rectX = tft.width() - rectW;
  if (rectY + rectH > tft.height()) rectY = tft.height() - rectH;

  // Draw new rectangle
  drawRectangle(rectX, rectY);
}

void printTouchToSerial(TS_Point p)
{
  Serial.print("Pressure = ");
  Serial.print(p.z);
  Serial.print(", x = ");
  Serial.print(p.x);
  Serial.print(", y = ");
  Serial.print(p.y);
  Serial.println();
}


void loop()
{
  if (ts.tirqTouched() && ts.touched())
  {
    TS_Point p = ts.getPoint();
    printTouchToSerial(p);
    drawRectangle(rectX, rectY);
    // printTouchToDisplay(p);
    delay(100);

    if (p.x < 1000 && p.y > 1000 && p.y < 3000)
    {
      Serial.println("left");
      moveRectangle("LEFT");
    }
    else if (p.x > 3000 && p.y > 1000 && p.y < 3000)
    {
      Serial.println("Right");
      moveRectangle("RIGHT");
    }
    else if (p.x > 1000 && p.x < 3000 && p.y < 1000)
    {
      Serial.println("Up");
      moveRectangle("UP");
    }
    else if (p.x > 1000 && p.x < 3000 && p.y > 3000)
    {
      Serial.println("Down");
      moveRectangle("DOWN");
    }
  }
  
}


//  void printTouchToDisplay(TS_Point p)
//   {

//     // Clear screen first
//     tft.fillScreen(TFT_BLACK);
//     tft.setTextColor(TFT_WHITE, TFT_BLACK);

//     int x = 320 / 2; // center of display
//     int y = 100;
//     int fontSize = 2;

//     String temp = "Pressure = " + String(p.z);
//     tft.drawCentreString(temp, x, y, fontSize);

//     y += 16;
//     temp = "X = " + String(p.x);
//     tft.drawCentreString(temp, x, y, fontSize);

//     y += 16;
//     temp = "Y = " + String(p.y);
//     tft.drawCentreString(temp, x, y, fontSize);
//   }