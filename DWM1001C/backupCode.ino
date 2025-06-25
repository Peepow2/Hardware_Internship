#define RX 16
#define TX 17
#define baud_rate 115200
#define ULONG_MAX 0xFFFFFFFFUL

byte cmd_Check_Valid_response[] = {0x40, 0x01, 0x00};

byte cmd_getposTAG[] = {0x02, 0x00};
byte cmd_getposAnchor[] = {0x0C, 0x00};
byte cmd_reset[] = {0x14, 0x00};
byte cmd_factory_reset[] = {0x13, 0x00};



byte* sent_cmd(byte cmd[], byte Size_of_cmd, byte ByteArrSize)
{
  // Wake up
  Serial2.write(0x00); 
  Serial2.flush();

  while(true)
  {
    unsigned long NOW = 0;
    unsigned long diff_from_LastSent = 0;
    unsigned long diff_from_begin = 0;
    unsigned long LastSent = millis();   // เวลาสำหรับการส่งใหม่
    unsigned long begin = millis();   // เวลาตั้งแต่เริ่มเรียกฟังก์ชัน

    Serial2.flush();
    while(!Serial2.available())
    {
      NOW = millis();
      diff_from_LastSent = \
          (NOW >= LastSent) ? (NOW - LastSent) : \
          (ULONG_MAX - LastSent + NOW + 1);

      if(diff_from_LastSent > 5000)
      {
        Serial2.flush();
        Serial2.write(cmd, Size_of_cmd);
        LastSent = millis();
        //Serial.println(diff_from_LastSent);
      }

      diff_from_begin = \
          (NOW > begin) ? NOW - begin : ULONG_MAX - begin + NOW + 1;

      if(diff_from_begin > 60000) // 1 min
      {
        // Something to stop everything
        Serial.println("End");
        return NULL;
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
      Serial.flush();
      return TLV; 
    }
    free(TLV);
    delay(100);
  }
}

float* pos_get()
{
  byte N = 20;
  byte *TLV = sent_cmd(cmd_getposTAG, sizeof(cmd_getposTAG), N);

  float* POS = (float*)malloc(3*sizeof(float));
  POS[0] = (TLV[5]  |  TLV[6] << 8 |  TLV[7] << 16 |  TLV[8] << 24) / 1000.0;
  POS[1] = (TLV[9]  | TLV[10] << 8 | TLV[11] << 16 | TLV[12] << 24) / 1000.0;
  POS[2] = (TLV[13] | TLV[14] << 8 | TLV[15] << 16 | TLV[16] << 24) / 1000.0;

  free(TLV);
  return POS;
}

/*void loc_get()
{
  byte cmd[] = {0x0B, 0x01, 0x01};
  byte *TLV = sent_cmd(cmd, sizeof(cmd), 10);
  for(byte i=0;i<6;i++)
  {
    Serial.print(TLV[i], BIN);
    Serial.print(" ");
  }
  Serial.println();
}*/

void setup() 
{
  Serial.begin(baud_rate);
  Serial2.begin(baud_rate, SERIAL_8N1, RX, TX);
}

void loop()
{
  float* Position_XYZ = pos_get();
  //loc_get();
  
  for(byte i=0;i<3;i++)
  {
    Serial.print(Position_XYZ[i]);
    Serial.print(" ");
  }
  Serial.println();
  delay(5000);
}
