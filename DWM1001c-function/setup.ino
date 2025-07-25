#define RX 27
#define TX 22
#define baud_rate 115200

void setup() 
{
  Serial.begin(baud_rate);
  Serial2.begin(baud_rate, SERIAL_8N1, RX, TX);
}

void loop() 
{

}
