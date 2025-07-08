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

}
