#define RX 16
#define TX 17
#define baud_rate 115200
#define ULONG_MAX 0xFFFFFFFFUL

byte cmd_Check_Valid_response[] = {0x40, 0x01, 0x00};

byte cmd_getposTAG[] = {0x02, 0x00};    // Tag Position
byte cmd_getposAnchor[] = {0x0C, 0x00}; // All location

byte cmd_sleep[] = {0x0A, 0x00};
byte cmd_reset[] = {0x14, 0x00};
byte cmd_factory_reset[] = {0x13, 0x00};

byte cmd_set_to_TAG[] = {0x05, 0x02, 0xCE, 0x00};
byte cmd_set_to_Anchor[] = {0x07, 0x02, 0x9E, 0x00};

byte cmd_get_Anchor_list[] = {0x0B,0x01,0x01};


byte* sent_cmd(byte cmd[])
{
  byte ByteArrSize = 125;

  unsigned long LastSent = millis();   // เวลาสำหรับการส่งใหม่
  unsigned long begin = millis();      // เวลาตั้งแต่เริ่มเรียกฟังก์ชัน
  unsigned long NOW = 0;
  unsigned long diff_from_LastSent = 0;
  unsigned long diff_from_begin = 0;

  
  while(!Serial2.available())
  {
    NOW = millis();
    diff_from_LastSent = \
        (NOW >= LastSent) ? (NOW - LastSent) : \
        (ULONG_MAX - LastSent + NOW);

    if(diff_from_LastSent > 5000)
    {
      Serial2.write(cmd, sizeof(cmd));
      LastSent = millis();
      Serial.println(diff_from_LastSent);
    }

    diff_from_begin = \
        (NOW > begin) ? NOW - begin : ULONG_MAX - begin + NOW;

    if(diff_from_begin > 60000) // 1 min
    {
      // Something to stop everything
      Serial.println("End");
    }
  }

  byte index = 0;
  byte *TLV = (byte*)malloc(ByteArrSize * sizeof(byte));
  for(byte i=0;i<ByteArrSize;i++)  TLV[i] = 0x00;

  while (Serial2.available() && index < ByteArrSize)
  {
    byte rev = Serial2.read();
    if(index > 0 || rev == cmd_Check_Valid_response[0])
    {
      TLV[index++] = rev;
    }
  }

  if(cmd_Check_Valid_response[0] == TLV[0] \
      && cmd_Check_Valid_response[1] == TLV[1] \
      && cmd_Check_Valid_response[2] == TLV[2])
  { 
    return TLV;
  }
}

float* pos_get()
{
  byte *TLV = sent_cmd(cmd_getposTAG);

  float* POS = (float*)malloc(3*sizeof(float));
  POS[0] = (TLV[5]  |  TLV[6] << 8 |  TLV[7] << 16 |  TLV[8] << 24) / 1000.0;
  POS[1] = (TLV[9]  | TLV[10] << 8 | TLV[11] << 16 | TLV[12] << 24) / 1000.0;
  POS[2] = (TLV[13] | TLV[14] << 8 | TLV[15] << 16 | TLV[16] << 24) / 1000.0;

  free(TLV);
  return POS;
}

void loc_get()
{
  byte N = 100;
  byte *TLV = sent_cmd(cmd_getposAnchor);
  for(byte i=0;i<N;i++)
  {
    Serial.print(TLV[i], HEX);
    Serial.print(" ");
  }
  Serial.println();
  
  free(TLV);
  //return POS;
}

void Reset()
{
  byte *TLV = sent_cmd(cmd_reset);
  if(cmd_Check_Valid_response[0] == TLV[0] \
    && cmd_Check_Valid_response[1] == TLV[1] \
    && cmd_Check_Valid_response[2] == TLV[2])
  {
    Serial.println("Reset Complete");
  }
  free(TLV);
}

void factory_reset()
{
  byte *TLV = sent_cmd(cmd_factory_reset);
  if(cmd_Check_Valid_response[0] == TLV[0] \
    && cmd_Check_Valid_response[1] == TLV[1] \
    && cmd_Check_Valid_response[2] == TLV[2])
  {
    Serial.println("Factory_reset Complete");
  }
  free(TLV);
}

void setup() 
{
  Serial.begin(baud_rate);
  Serial2.begin(baud_rate, SERIAL_8N1, RX, TX);
}

void loop()
{
  float* Position_XYZ = pos_get();

  //loc_get();
  
  /*for(byte i=0;i<3;i++)
  {
    Serial.print(Position_XYZ[i]);
    Serial.print(" ");
  }
  Serial.println();*/
  delay(2000);
}
