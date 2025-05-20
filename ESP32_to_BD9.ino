#define Rx 16
#define Tx 17

void setup() 
{
  Serial.begin(115200); // USB
  Serial2.begin(9600, SERIAL_8N1, Rx, Tx); // Blue DB9
  Serial.println("ESP32 UART2 (GPIO16/17) ready.");
}

void loop() 
{
  if (Serial2.available()) 
  {
    String received = Serial2.readString();
    Serial.print("From CH340: ");
    Serial.println(received);
  }
}
