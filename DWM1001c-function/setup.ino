#define RX 16
#define TX 17
#define baud_rate 115200

byte cmd_Check_Valid_response[] = {0x40, 0x01, 0x00};

void setup() 
{
  Serial.begin(baud_rate);
  Serial2.begin(baud_rate, SERIAL_8N1, RX, TX);
}

void loop() 
{

}
