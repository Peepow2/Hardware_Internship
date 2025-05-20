#include <HardwareSerial.h>

#define Rx 16
#define Tx 17

HardwareSerial mySerial(2);

void setup() 
{
  Serial.begin(115200);
  mySerial.begin(9600, SERIAL_8N1, Rx, Tx);
  Serial.println("ESP32 UART2 (GPIO16/17) ready.");
}

void loop() 
{
  if (mySerial.available()) 
  {
    String received = mySerial.readStringUntil('\n');
    Serial.print("From CH340: ");
    Serial.println(received);
  }
}
