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
byte cmd_get_All_loc[] = {0x0C, 0x00};

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

struct location
{
  float x = 0.0, y = 0.0, z = 0.0;
  float dist = 0.0;
  byte N_anchor = 0;
  String Label_Name = "";
};

struct location* get_loc()
{
  byte* TLV = send_cmd(cmd_get_All_loc);
  byte Num_Anchor = TLV[20];

  struct location* loc = \
              (struct location*)malloc((Num_Anchor+1)*sizeof(struct location));

  loc[0].x = (TLV[5]  |  TLV[6] << 8 |  TLV[7] << 16 |  TLV[8] << 24) / 1000.0;
  loc[0].y = (TLV[9]  | TLV[10] << 8 | TLV[11] << 16 | TLV[12] << 24) / 1000.0;
  loc[0].z = (TLV[13] | TLV[14] << 8 | TLV[15] << 16 | TLV[16] << 24) / 1000.0;
  loc[0].N_anchor = Num_Anchor;

  for(byte i=1;i<=Num_Anchor;i++)
  {
    loc[i].x = (TLV[20*i+8] | TLV[20*i+9] << 8 |\
                   TLV[20*i+10] << 16 | TLV[20*i+11] << 24)/1000.0;

    loc[i].y = (TLV[20*i+12] | TLV[20*i+13] << 8 |\
                   TLV[20*i+14] << 16 | TLV[20*i+15] << 24)/1000.0;

    loc[i].z = (TLV[20*i+16] | TLV[20*i+17] << 8 |\
                   TLV[20*i+18] << 16 | TLV[20*i+19] << 24)/1000.0;
  }
  free(TLV);
  return loc;
}

void printToDisplay(struct location* loc) 
{
  int margin = 20;
  float maxX = loc[1].x;
  float maxY = loc[1].y;
  float minX = loc[1].x;
  float minY = loc[1].y;

  for(byte i=1;i<=loc[0].N_anchor;i++)
  {
    maxX = max(maxX, loc[i].x);
    maxY = max(maxY, loc[i].y);

    minX = min(minX, loc[i].x);
    minY = min(minY, loc[i].y);
  }

  int TAG_X = map(loc[0].x, minX, maxX, 0+margin, SCREEN_WIDTH-margin);
  int TAG_Y = map(loc[0].y, minY, maxY, SCREEN_HEIGHT-margin, 0+margin);

  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE);

  for(byte i=1;i<=loc[0].N_anchor;i++)
  {
    int x = map(loc[i].x, minX, maxX, 0+margin, SCREEN_WIDTH-margin);
    int y = map(loc[i].y, minY, maxY, SCREEN_HEIGHT-margin, 0+margin);

    tft.drawLine(TAG_X, TAG_Y, x, y, TFT_GREEN);
    tft.fillSmoothCircle(x, y, 5, TFT_WHITE);
    tft.drawCentreString(String(i), x, y+8, FONT_SIZE);
  }
  tft.fillSmoothCircle(TAG_X, TAG_Y, 5, TFT_RED);
  tft.drawCentreString("TAG", TAG_X, TAG_Y+8, FONT_SIZE);

  /*for(byte i=0;i<=loc[0].N_anchor;i++)
  {
    Serial.print(loc[i].x); Serial.print(" ");
    Serial.print(loc[i].y); Serial.print(" ");
    Serial.print(loc[i].z); Serial.println("\n-------------");
  }*/
}

void setup() 
{
  Serial.begin(baud_rate);
  Serial2.begin(baud_rate, SERIAL_8N1, RX, TX);

  tft.init();
  tft.setRotation(2);
}

void loop() 
{
  struct location* loc = get_loc();
  printToDisplay(loc);

  free(loc);
}
