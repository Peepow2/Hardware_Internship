#define RX 16
#define TX 17
#define baud_rate 115200

byte cmd_Check_Valid_response[] = {0x40, 0x01, 0x00};

byte cmd_factory_reset[] = {0x13, 0x00};
byte cmd_reset[] = {0x14, 0x00};

byte head_cmd_set_to_Anchor[] = {0x07, 0x02};
byte head_cmd_set_label[] = {0x1D};
byte head_cmd_set_Bluetooth_Address[] = {0x0F, 0x06};

byte cmd_get_config[] = {0x08, 0x00};

byte min(byte a, byte b)
{return (a<b) ? a : b;}

byte* sent_cmd(byte cmd[])
{
  byte ByteArrSize = 100;
  byte *TLV = (byte*)malloc(ByteArrSize * sizeof(byte));

  byte N = 5, TimeOUT = 5;
  while(N--)
  {
    byte index = 0;
    for(byte i=0;i<ByteArrSize;i++)  TLV[i] = 0x00;

    byte sentcmd_Count = 0;
    Serial2.flush();

    while(sentcmd_Count <= TimeOUT && !Serial2.available())
    {
      Serial2.write(cmd, sizeof(byte) * (cmd[1] + 2));
      sentcmd_Count++;
      delay(1000);
      //Serial.print(sentcmd_Count);
    }

    if(sentcmd_Count > TimeOUT && !Serial2.available())
      continue;

    while (Serial2.available() && index < ByteArrSize)
    {
      byte rev = Serial2.read();
      if(index > 0 || rev == cmd_Check_Valid_response[0])
      {
        TLV[index++] = rev;
      }
    }

    for(byte i=0;i<index;i++)
    {
      Serial.print(TLV[i], HEX);
      Serial.print(" ");
    }
    Serial.println();

    if(cmd_Check_Valid_response[0] == TLV[0] \
        && cmd_Check_Valid_response[1] == TLV[1] \
        && cmd_Check_Valid_response[2] == TLV[2])
    { 
      return TLV;
    }
  }
  free(TLV);
  return NULL;
}

bool set_Bluetooth_Address(String _Address)
{
  byte* cmd_set_Bluetooth_Address = (byte*)malloc(8*sizeof(byte));
  for(byte i=0;i<8;i++) cmd_set_Bluetooth_Address[i] = 0x00;

  // {0x0F, 0x06, First 6 char from Address in HEX}
  cmd_set_Bluetooth_Address[0] = head_cmd_set_Bluetooth_Address[0];
  cmd_set_Bluetooth_Address[1] = head_cmd_set_Bluetooth_Address[1];

  byte len = min(_Address.length(), (byte)6);
  for(byte i=0;i<len;i++)
    cmd_set_Bluetooth_Address[i+2] = (byte)_Address.charAt(i); 

  /*for(byte i=0;i<8;i++)
  {
    Serial.print(cmd_set_Bluetooth_Address[i], HEX);
    Serial.print(" ");
  }Serial.println();*/

  free(sent_cmd(cmd_set_Bluetooth_Address));
  free(sent_cmd(cmd_reset));
  free(cmd_set_Bluetooth_Address);
  return true;
}

bool set_label(String L)
{
  bool complete = false;

  byte* cmd_set_label = (byte*)malloc((L.length()+2)*sizeof(byte));
  cmd_set_label[0] = head_cmd_set_label[0];
  cmd_set_label[1] = L.length();

  for(byte i=0;i<L.length();i++)
  {
    cmd_set_label[i+2] = L.charAt(i);
  }

  byte* received = sent_cmd(cmd_set_label);
  byte* Reset = sent_cmd(cmd_reset);

  if(received != NULL && Reset != NULL) complete = true;

  free(received);
  free(Reset);
  free(cmd_set_label);

  return complete;
}


bool set_to_Anchor(byte initiator, byte bridge, byte enc_en, byte led_en, \
                    byte ble_en, byte fw_update_en, byte uwb_mode)
{
  bool complete = false;
  
  byte BYTE0 = initiator << 7 | bridge << 6 | enc_en << 5 | \
                 led_en << 4 | ble_en << 3 | fw_update_en << 2 | uwb_mode;
  byte BYTE1 = 0b00;

  // {0x07, 0x02, BYTE0, BYTE1}
  byte* cmd_set_to_Anchor = (byte*)malloc(4*sizeof(byte));
  cmd_set_to_Anchor[0] = head_cmd_set_to_Anchor[0];
  cmd_set_to_Anchor[1] = head_cmd_set_to_Anchor[1];
  cmd_set_to_Anchor[2] = BYTE0;
  cmd_set_to_Anchor[3] = BYTE1;

  byte* Anchor_set = sent_cmd(cmd_set_to_Anchor);
  free(cmd_set_to_Anchor);  
  
  if(Anchor_set != NULL) 
  {
    free(Anchor_set);

    byte* config = sent_cmd(cmd_get_config);
    if(config != NULL)
    {
      if((config[6] & 0b00100000) == 0b00100000)
      {       
        Serial.println("Anchor Complete");
        complete = true;
      }
    }
    free(config);
  }
  byte* reset = sent_cmd(cmd_reset); free(reset);
  return complete;
}

void setup() 
{
  Serial.begin(baud_rate);
  Serial2.begin(baud_rate, SERIAL_8N1, RX, TX);
}

void loop() 
{
  String Label = "RND_ROOM";
  String Address = "Judjod"; // Valid for first 6 chars

  byte initiator = 1;
  byte bridge = 0;
  byte enc_en = 0;
  byte led_en = 1;
  byte ble_en = 1;
  byte fw_update_en = 1;
  byte uwb_mode = 0b10;

  bool complete1 = set_to_Anchor(initiator, bridge, enc_en, led_en, \
                                  ble_en, fw_update_en, uwb_mode);
  bool complete2 = set_label(Label);
  bool complete3 = set_Bluetooth_Address(Address);

  if(complete1 && complete2 && complete3) 
  {Serial.print("complete"); while(complete2);}
  delay(5000);
}
