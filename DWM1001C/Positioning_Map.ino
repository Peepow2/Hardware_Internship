#define baud_rate 115200
#define Rx 16
#define Tx 17

int count_NumIndex_ByteArray(byte TYPE)
{
  if(TYPE == 0x00) sent_wake_up_protocols();
  if(TYPE == 0x02) sent_pos_get_protocols();
  if(TYPE == 0x0C) sent_loc_get_protocols();

  int N = 0;
  while (Serial2.available())
  {
    byte res = Serial2.read();
    N++;
  }
  return N;
}

bool valid_protocol(byte p[])
{
  if (!(p[0] == 0x40 && p[1] == 0x01 && p[2] == 0x00))
    return false;
  return true;
}

void clear_protocol()
{
  while(Serial2.available()) byte r = Serial2.read();
}


void sent_wake_up_protocols()
{
  Serial2.write(0x00);
  delay(100);
}

void sent_loc_get_protocols()
{
  byte loc_get[] = {0x0C, 0x00};
  Serial2.write(loc_get, sizeof(loc_get));
  delay(100);
}

void sent_pos_get_protocols()
{
  byte pos_get[] = {0x02, 0x00};
  Serial2.write(pos_get, sizeof(pos_get));
  delay(100);
}

float* pos_get()
{ 
  sent_wake_up_protocols(); clear_protocol();
  sent_pos_get_protocols();

  while (!Serial2.available())
  {
    if(millis() - Time_out > 5000)
    {
      clear_protocol();
      int Time_out = millis();
      sent_pos_get_protocols();
    }
  }

  float* position_XYZ = (float*)malloc(3*sizeof(float));

  position_XYZ[0] = 0.0;
  position_XYZ[1] = 0.0;
  position_XYZ[2] = 0.0;

  byte idx = 0, TLV[20];
  for(int i=0;i<20;i++) TLV[i] = 0x00;

  do
  {
    byte res = Serial2.read();
    if(idx > 0 || res == 0x40)
    {
      TLV[idx] = res;
      idx++;
    }
  }while (Serial2.available() && idx < 20);


  if(valid_protocol(TLV))
  {
    /*for(int i=0;i<18;i++)
    {
      Serial.print(TLV[i], HEX);
      Serial.print(" ");
    }
    Serial.println();*/

    for(int i=0;i<3;i++)
    {
      int x = 0; 
      for(int j=0;j<4;j++)
      {
        x = TLV[4*i+j+5] << (8*j) | x;
      }
      position_XYZ[i] = x/1000.0;
    }
  }

  for(int i=0;i<3;i++)
  {
    Serial.print(position_XYZ[i]);
    Serial.print(" ");
  }
  Serial.println();
  Serial.println("-------------");
  return position_XYZ;
}

void loc_get()
{
  sent_wake_up_protocols(); //clear_protocol();
  int N = count_NumIndex_ByteArray(0x0C);

  byte N_anchor = byte((N-21)/20);
  byte arr_size = N + 5;

  byte* TLV = (byte*)malloc(arr_size * sizeof(byte));
  sent_loc_get_protocols();

  int Time_out = millis();
  while (!Serial2.available())
  {
    if(millis() - Time_out > 5000)
    {
      Time_out = millis();
      sent_loc_get_protocols();
    }
  }

  //for(int i=0;i<arr_size;i++) TLV[i] = 0x00;

  byte idx = 0;
  do
  {
    byte res = Serial2.read();
    if(idx > 0 || res == 0x40)
    {
      TLV[idx] = res;
      idx++;
    }
  }while(Serial2.available() && idx < arr_size);

  /*for(int i=0;i<arr_size;i++)
  {
    Serial.print(TLV[i], HEX);
    Serial.print(" ");
  }
  Serial.println();*/


  for(int i=0;i<N_anchor;i++)
  {
    for(int j=0;j<3;j++)
    {
      int x = 0;
      for(int k=0;k<4;k++)
      {
        x = (TLV[20*(i+1)+4*j+k+8] << (8*k)) | x;
      }
     Serial.print(x/1000.0);
     Serial.print(" ");
    }
    int dist = 0;
    for(int j=0;j<4;j++)
    {
      dist = (TLV[20*(i+1)+j+3] << (8*j)) | dist;
    }
    Serial.print(" --> ");
    Serial.println(dist/1000.0);
  }
  Serial.println();
}


void setup() 
{
  Serial.begin(baud_rate);
  Serial2.begin(baud_rate, SERIAL_8N1, Rx, Tx);
}

void loop() 
{
  float* position = pos_get();
  loc_get();

  delay(5000);
}
