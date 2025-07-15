#include <TFT_eSPI.h>

TFT_eSPI tft = TFT_eSPI();

#define SCREEN_WIDTH 240
#define SCREEN_HEIGHT 320
#define FONT_SIZE 1

#define RX 27
#define TX 22
#define baud_rate 115200

byte cmd_Check_Valid_response[] = {0x40, 0x01, 0x00};
byte cmd_getposTAG[] = {0x02, 0x00};

float* get_pos()
{
  byte* TLV = send_cmd(cmd_getposTAG);
  if(TLV == NULL) return NULL;

  float* POS = (float*)malloc(3*sizeof(float));
  POS[0] = (TLV[5]  |  TLV[6] << 8 |  TLV[7] << 16 |  TLV[8] << 24) / 1000.0;
  POS[1] = (TLV[9]  | TLV[10] << 8 | TLV[11] << 16 | TLV[12] << 24) / 1000.0;
  POS[2] = (TLV[13] | TLV[14] << 8 | TLV[15] << 16 | TLV[16] << 24) / 1000.0;

  free(TLV);
  return POS;
}

byte* send_cmd(byte cmd[])
{
  byte ByteArrSize = 150;
  byte *TLV_temp = (byte*)malloc(ByteArrSize * sizeof(byte));

  byte N = 5, TimeOUT = 5;
  while(N--)
  {
    byte index = 0;
    for(byte i=0;i<ByteArrSize;i++)  TLV_temp[i] = 0x00;

    byte sentcmd_Count = 0;
    Serial2.flush();

    while(sentcmd_Count <= TimeOUT && !Serial2.available())
    {
      Serial2.write(cmd, (cmd[1]+2)*sizeof(byte));
      sentcmd_Count++;
      delay(1000);
    }

    if(sentcmd_Count > TimeOUT && !Serial2.available())
      continue;

    while (Serial2.available() && index < ByteArrSize)
    {
      byte rev = Serial2.read();
      if(index > 0 || rev == cmd_Check_Valid_response[0])
      {
        TLV_temp[index++] = rev;
      }
    }

    if(cmd_Check_Valid_response[0] == TLV_temp[0] \
        && cmd_Check_Valid_response[1] == TLV_temp[1] \
        && cmd_Check_Valid_response[2] == TLV_temp[2])
    {
      byte *TLV = (byte*)malloc(index * sizeof(byte));
      for(byte i=0;i<index;i++)
        TLV[i] = TLV_temp[i];
      free(TLV_temp);
      return TLV;
    }
  }
  free(TLV_temp);
  return NULL;
}

void printToDisplay(float x, float y, float z) 
{
  tft.fillScreen(TFT_WHITE);
  tft.setTextColor(TFT_BLACK);

  int centerX = SCREEN_WIDTH / 2;
  int centerY = SCREEN_HEIGHT / 2;

  int _x = 10; // x-coordinate of the top-left corner of the rectangle
  int _y = 10; // y-coordinate of the top-left corner of the rectangle
  int _w = 10; // width of the rectangle
  int _h = 10; // height of the rectangle
  uint16_t color = TFT_RED; // color of the rectangle

  tft.fillRect(_x, _y, _w, _h, color);

  tft.drawLine(_x+15, _y+15, centerX-10, centerY-25, TFT_GREEN);

  tft.fillSmoothCircle(centerX, centerY-15, 4, TFT_RED);
  String S = "(" + String(x) + ", " + String(y) + ", " + String(z) + ")";
  tft.drawCentreString(S, centerX, centerY, FONT_SIZE);
}

void setup() 
{
  Serial.begin(baud_rate);
  Serial2.begin(baud_rate, SERIAL_8N1, RX, TX);

  tft.init();
  tft.setRotation(2);

  tft.fillScreen(TFT_WHITE);
  tft.setTextColor(TFT_BLACK);
}

void loop() 
{
  float* POS = get_pos();
  printToDisplay(POS[0], POS[1], POS[2]);
  delay(2000);
}
