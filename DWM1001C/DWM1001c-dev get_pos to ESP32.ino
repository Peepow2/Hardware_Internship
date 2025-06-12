#define baud_rate 115200
#define Rx 16
#define Tx 17

int x, y, z, prev_x = 0, prev_y = 0, prev_z = 0;

void wake_up()
{
  Serial2.write(0x00);
  delay(200);
}

bool valid_protocol(byte p[])
{
  if (!(p[0] == 0x40 && p[1] == 0x01 && p[2] == 0x00))
    return false;
  return true;
}

void setup() 
{
  Serial.begin(baud_rate);
  Serial2.begin(baud_rate, SERIAL_8N1, Rx, Tx);

  wake_up();
}

void loop() 
{
  byte pos_get[] = {0x02, 0x00};
  Serial2.write(pos_get, sizeof(pos_get));
  delay(100);

  while (!Serial2.available()) {} // Time out

  byte idx = 0, TLV[20];
  //for(int i=0;i<20;i++) TLV[i] = 0x00

  do
  {
    byte res = Serial2.read(); // ค่าที่ Serial2 ส่งมา
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

    x = TLV[5]  | (TLV[6] << 8)  | (TLV[7] << 16)  | (TLV[8] << 24);
    y = TLV[9]  | (TLV[10] << 8) | (TLV[11] << 16) | (TLV[12] << 24);
    z = TLV[13] | (TLV[14] << 8) | (TLV[15] << 16) | (TLV[16] << 24);

    if(prev_x != x && prev_y != y && prev_z != z)
    {
      Serial.print(x/1000.0);    Serial.print(" ");
      Serial.print(y/1000.0);    Serial.print(" ");
      Serial.println(z/1000.0);

      prev_x = x, prev_y = y, prev_z = z;
    }
    delay(1000);
  }
}
