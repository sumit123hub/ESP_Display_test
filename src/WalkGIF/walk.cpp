// #include "walk.h"

// #include <SPI.h>
// #include <TFT_eSPI.h> // Hardware-specific library
// #include <Arduino.h>

// TFT_eSPI tft = TFT_eSPI();

// // Frame buffer for one frame (RAM)
// uint16_t frameBuffer[6700]; // animation_width * animation_height

// void setup() {
//   Serial.begin(115200);
//   tft.init();
//   tft.setRotation(3);
//   tft.setSwapBytes(true);
//   tft.fillScreen(TFT_WHITE);
// }

// void loop() {
//   for (int i = 0; i < frames; i++) {
//     // Copy one frame from PROGMEM to RAM
//     // Dont try to directly push from progmem to display it wouldnt work as pushImage requires pointer to ram . Many libraries like TJpg_Decoder.h use this method. They first copy into tiles in the ram and use buffering to push them to the display using DMA
//     for (int j = 0; j < animation_width * animation_height; j++) {
//       frameBuffer[j] = pgm_read_word(&(walk[i][j]));
//     }

//     // Draw the frame at position (60, 15)
//     tft.pushImage(60, 15, animation_width, animation_height, frameBuffer);

//     // Delay for ~25 FPS
//     delay(40);
//   }
// }
