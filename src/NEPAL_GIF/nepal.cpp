// #include "nepal.h"

// #include <Arduino.h>
// #include <SPI.h>
// #include <TFT_eSPI.h> // Hardware-specific library

// TFT_eSPI tft = TFT_eSPI();

// // Frame buffer for one frame (RAM)
// // Using this technique will cause a huge issue as if you declare this much variables
// // then it will be stored in .bss section of ram which will store unintitialized values due to which compilation error will occur
// // there are many techniwues to work around this and one of them is same like jpeg image using tile buffering and dma and other similar ways.

// uint16_t frameBuffer[50000]; // animation_width * animation_height



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
//       frameBuffer[j] = pgm_read_word(&(frame[i][j]));
//     }

//     // Draw the frame at position (60, 15)
//     tft.pushImage(0, 0, animation_width, animation_height, frameBuffer);

//     // Delay for ~25 FPS
//     delay(40);
//   }
// }
