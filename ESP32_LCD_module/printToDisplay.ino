#include <TFT_eSPI.h>

TFT_eSPI tft = TFT_eSPI();

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240
#define FONT_SIZE 1

void printToDisplay(String S) 
{
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE);

  tft.drawCentreString(S, X_coor, Y_coor, FONT_SIZE);
}

void setup() 
{
  Serial.begin(115200);

  tft.init();
  tft.setRotation(2); // 1 Landscape, 2 Portrait
}

void loop() 
{
  printToDisplay("Something");
}
