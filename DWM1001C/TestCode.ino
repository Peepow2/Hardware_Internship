#define RX 16
#define TX 17
#define baud_rate 115200

void setup() 
{
  Serial.begin(baud_rate);
  Serial2.begin(baud_rate, SERIAL_8N1, RX, TX);
}

void loop()
{
  byte cmd[] = {0x02, 0x00};
  Serial2.write(cmd, sizeof(cmd));
  delay(100);

  while(Serial2.available())
  {
    byte res = Serial2.read();
    Serial.print(res, HEX);
    Serial.print(" ");
  }
  Serial.println();
  delay(2000);
}
