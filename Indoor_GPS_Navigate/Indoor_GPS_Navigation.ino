#include <TFT_eSPI.h>
#include <SPI.h>
#include <XPT2046_Touchscreen.h>

#define ESP32LCD Serial
#define DWW1001c Serial2

#define RX 27
#define TX 22
#define baudrate 115200

#define XPT2046_IRQ 36   // T_IRQ
#define XPT2046_MOSI 32  // T_DIN
#define XPT2046_MISO 39  // T_OUT
#define XPT2046_CLK 25   // T_CLK
#define XPT2046_CS 33    // T_CS

#define SCREEN_WIDTH 240
#define SCREEN_HEIGHT 320
#define Margin 20
#define FONT_SIZE 1
#define FONT_COLOR 0x0000
#define BG_COLOR 0x07FF

#define WINDOW 0xCC06  /* 0xCC06, 0d55230*/
#define HAWK  0xD89B   /* 0xD89B, 0d55439*/
#define WALL 0xCD12    /* 0xCD12, 0d52498*/
#define DOOR 0xD88F    /* 0xD88F, 0d55439*/

TFT_eSPI tft = TFT_eSPI();
SPIClass touchscreenSPI = SPIClass(VSPI);
XPT2046_Touchscreen touchscreen(XPT2046_CS, XPT2046_IRQ);
// ------------------- Defined variable type ------------------- //
struct location
{
  float x = 0.0, y = 0.0, z = 0.0;
  float dist = 0.0;
  byte N_anchor = 0;
  uint16_t UWB_Address = 0;
};
// ------------------------------------------------------------- //

// ------------------- Necessary Function ------------------- //

byte cmd_Check_Valid_response[] = {0x40, 0x01, 0x00};
byte cmd_get_loc[] = {0x0C, 0x00};

byte* send_cmd(byte cmd[])
{
  if(!connected(DWW1001c))
    return NULL;

  byte ByteArrSize = 150;
  byte *TLV_temp = (byte*)malloc(ByteArrSize * sizeof(byte));

  byte N = 5, TimeOUT = 5;
  while(N--)
  {
    byte index = 0;
    for(byte i=0;i<ByteArrSize;i++)  TLV_temp[i] = 0x00;

    byte sentcmd_Count = 0;
    DWW1001c.flush();

    while(sentcmd_Count <= TimeOUT && !DWW1001c.available())
    {
      DWW1001c.write(cmd, (cmd[1]+2)*sizeof(byte));
      sentcmd_Count++;
      delay(800);
    }

    if(sentcmd_Count > TimeOUT && !DWW1001c.available())
      continue;

    while (DWW1001c.available() && index < ByteArrSize)
    {
      byte rev = DWW1001c.read();
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

struct location* get_loc()
{
  byte* TLV = send_cmd(cmd_get_loc);
  if (TLV == NULL) return NULL;

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

    loc[i].UWB_Address = (uint16_t)(TLV[20*i+1] | (TLV[20*i+2] << 8));
  }
  free(TLV);
  return loc;
}

void Show_location_LCD(struct location* loc) 
{
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

  int X = map_value(loc[0].x, minX, maxX, 0+Margin, SCREEN_WIDTH-Margin);
  int Y = map_value(loc[0].y, minY, maxY, SCREEN_HEIGHT-Margin, 0+Margin);

  tft.fillScreen(BG_COLOR);
  tft.setTextColor(FONT_COLOR);

  for(byte i=0;i<=loc[0].N_anchor;i++)
  {
    int X = map_value(loc[i].x, minX, maxX, 0+Margin, SCREEN_WIDTH-Margin);
    int Y = map_value(loc[i].y, minY, maxY, SCREEN_HEIGHT-Margin, 0+Margin);
    if(i==0)
    {
      tft.fillSmoothCircle(X, Y, 5, TFT_GREEN, TFT_GREEN);
      tft.drawCentreString("YOU", X, Y+10, 1);
    }
      
    else
    {
      tft.fillCircle(X, Y, 5, TFT_BLACK);
      String Position_Name = "";
      if(loc[i].UWB_Address == WINDOW)
        Position_Name = "WINDOW";
      if(loc[i].UWB_Address == HAWK)
        Position_Name = "HAWK";
      if(loc[i].UWB_Address == WALL)
        Position_Name = "WALL";
      if(loc[i].UWB_Address == DOOR)
        Position_Name = "DOOR";
      tft.drawCentreString(Position_Name, X, Y+10, 1);
    }
  }
}

void Warning_message_LCD()
{
  tft.fillScreen(BG_COLOR);
  tft.setTextColor(FONT_COLOR);
  tft.drawCentreString("Check your connection", SCREEN_WIDTH/2, \
                          SCREEN_HEIGHT/2 - 30, 2);
  tft.drawRect(70, 160, 100, 40, 0xD69A); // x, y, x_length, y_length
  tft.drawCentreString("Retry", 120, 170, 2);
}

bool Press_Retry()
{
  if (touchscreen.tirqTouched() && touchscreen.touched()) 
  {
    TS_Point p = touchscreen.getPoint();
    while(p.z >= 10) 
    {
      p = touchscreen.getPoint();
    }
    tft.fillScreen(BG_COLOR);
    int x = map(p.x, 200, 3700, 1, SCREEN_WIDTH);
    int y = map(p.y, 240, 3800, 1, SCREEN_HEIGHT);

    return (x>=70 && x<=170 && y>=160 && y<=200);
  }
  return false;
}
// ---------------------------------------------------------- //


// ------------------- Additional Function ------------------ // 
float min(float a, float b)
{return (a<b)?a:b;}

float max(float a, float b)
{return (a>b)?a:b;}

// A = fromLow, B = fromHigh, C = toLow, D = toHigh
int map_value(float value, float A, float B, float C, float D)
{return (A!=B)?int((value-A)*(D-C)/(B-A)+C):A;}

void flushBuffer(Stream &serialPort)
{while(serialPort.available()) byte r = serialPort.read();}

bool connected(Stream &serialPort)
{
  bool connected_SerialPort = false;

  byte test[] = {0x00}; 
  serialPort.write(test, sizeof(test)); delay(100);

  if(serialPort.available())
  {
    byte rev = serialPort.read();
    while(serialPort.available() && rev != 0x40)
      { rev = serialPort.read();}

    connected_SerialPort = true;
    for(byte i=0;i<2;i++)
    {
      if(!serialPort.available() || serialPort.read() != 0x01)
      { connected_SerialPort = false;}
    }
  }
  flushBuffer(serialPort);
  return connected_SerialPort;
}
// ---------------------------------------------------------- // 

void setup() 
{
  ESP32LCD.begin(baudrate);
  DWW1001c.begin(baudrate, SERIAL_8N1, RX, TX);

  tft.init();
  tft.invertDisplay(true);
  tft.setRotation(2);
  tft.fillScreen(BG_COLOR);

  touchscreenSPI.begin(XPT2046_CLK, XPT2046_MISO, XPT2046_MOSI, XPT2046_CS);
  touchscreen.begin(touchscreenSPI);
  touchscreen.setRotation(2);
}

void loop() 
{
  struct location* loc = get_loc();
  
  if(loc == NULL)
  {
    Warning_message_LCD();
    while(!Press_Retry()) {}
  }
  else
  {
    Show_location_LCD(loc);
  }
  free(loc);
}
