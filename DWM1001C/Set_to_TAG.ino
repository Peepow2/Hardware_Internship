#define RX 16
#define TX 17
#define baud_rate 115200

byte cmd_Check_Valid_response[] = {0x40, 0x01, 0x00};

byte cmd_reset[] = {0x14, 0x00};
byte cmd_set_to_TAG[] = {0x05, 0x02, 0xCE, 0x00};

byte cmd_get_config[] = {0x08, 0x00};


byte* sent_cmd(byte cmd[])
{
  byte ByteArrSize = 50;

  Serial2.write(cmd, sizeof(cmd));
  delay(100);

  unsigned long LastSent = millis();   // เวลาสำหรับการส่งใหม่
  unsigned long begin = millis();      // เวลาตั้งแต่เริ่มเรียกฟังก์ชัน
  unsigned long NOW = 0;
  unsigned long diff_from_LastSent = 0;
  unsigned long diff_from_begin = 0;

  while(!Serial2.available())
  {
    NOW = millis();
    diff_from_LastSent = NOW - LastSent;
    diff_from_begin    = NOW - begin;

    if(diff_from_LastSent > 5000)
    {
      Serial2.write(cmd, sizeof(cmd));
      delay(100);
      LastSent = millis();
      Serial.println(diff_from_LastSent);
    }

    else if(diff_from_begin > 60000) // 1 min
    {
      // Something to stop everything
      Serial.println("End");
      delay(200000);
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
      Serial.print(rev);
      Serial.print(" ");
    }
  }
  Serial.println();

  if(cmd_Check_Valid_response[0] == TLV[0] \
      && cmd_Check_Valid_response[1] == TLV[1] \
      && cmd_Check_Valid_response[2] == TLV[2])
  { 
    for(byte i=0;i<index;i++)
    {
      Serial.print(TLV[i]);
      Serial.print(" ");
    }Serial.println();
    return TLV;
  }
}

void set_node_to_TAG(String Label, String Address, int update_rate)
{
  
}

void setup() 
{
  Serial.begin(baud_rate);
  Serial2.begin(baud_rate, SERIAL_8N1, RX, TX);
}

void loop() 
{
  String Label = "HOK";
  String Address = "Infinite";
  int update_rate = 1; // x 100 ms
  set_node_to_TAG(Label, Address, update_rate);
  delay(20000);
}
