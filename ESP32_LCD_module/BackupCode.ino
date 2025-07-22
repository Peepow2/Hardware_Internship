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

void Show_location_LCD(struct location* loc) 
{
  /*for(byte i=0;i<=loc[0].N_anchor;i++)
  {
    if(i==0)  
      Serial.println("TAG");
    else if(i==1)
      Serial.println("Anchor");
    ESP32LCD.print(loc[i].x); ESP32LCD.print(" ");
    ESP32LCD.print(loc[i].y); ESP32LCD.print(" ");
    ESP32LCD.print(loc[i].z); ESP32LCD.println();
  }ESP32LCD.println("-------------");*/

  float minX = loc[1].x, maxX = loc[1].x;
  float minY = loc[1].y, maxY = loc[1].y;
  float minZ = loc[1].z, maxZ = loc[1].z;

  for(byte i=2;i<=loc[0].N_anchor;i++)
  {
    minX = min(minX, loc[i].x);
    minY = min(minY, loc[i].y);
    minZ = min(minZ, loc[i].z);

    maxX = max(maxX, loc[i].x);
    maxY = max(maxY, loc[i].y);
    maxZ = max(maxZ, loc[i].z);
  }
  /*ESP32LCD.print(minX); ESP32LCD.print(" ");
  ESP32LCD.print(minY); ESP32LCD.print(" ");
  ESP32LCD.print(minZ); ESP32LCD.println();

  ESP32LCD.print(maxX); ESP32LCD.print(" ");
  ESP32LCD.print(maxY); ESP32LCD.print(" ");
  ESP32LCD.print(maxZ); ESP32LCD.println("");
  ESP32LCD.println("-------------");*/
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
  Show_location_LCD(loc);

  free(loc);
}
