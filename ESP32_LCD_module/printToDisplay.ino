#include <TFT_eSPI.h>

TFT_eSPI tft = TFT_eSPI();

#define SCREEN_WIDTH 240
#define SCREEN_HEIGHT 320
#define FONT_SIZE 2

void printToDisplay(String S) 
{
  tft.fillScreen(TFT_WHITE);
  tft.setTextColor(TFT_BLACK);

  int centerX = SCREEN_WIDTH / 2;
  int centerY = SCREEN_HEIGHT / 2;

  tft.drawCentreString(S, centerX, centerY, FONT_SIZE);
}

void setup() 
{
  Serial.begin(115200);

  tft.init();
  tft.setRotation(2);

  tft.fillScreen(TFT_WHITE);
  tft.setTextColor(TFT_BLACK);
}

void loop() 
{
  printToDisplay(millis());
}
