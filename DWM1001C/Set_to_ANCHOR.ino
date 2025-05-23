#define RX 16
#define TX 17

void sendCommand(String cmd) 
{
  Serial1.print(cmd + "\r");
  delay(200);
  while (Serial1.available()) 
  {
    btye c = Serial1.read();
    Serial.print(c);
  }
}

void setup() 
{
  Serial.begin(115200);
  Serial1.begin(115200, SERIAL_8N1, RX, TX); // RX=GPIO16, TX=GPIO17

  Serial.println("ตั้งค่า DWM1001C เป็น ANCHOR");

  sendCommand("nma");   // ตั้งเป็น ANCHOR
  sendCommand("apg");   // เปิด Auto Positioning (Anchor จะรู้ตำแหน่งตัวเอง)
  sendCommand("reset"); // รีเซ็ตเพื่อเริ่มทำงาน
  delay(3000);          // รอให้โมดูลบู๊ตใหม่
}

void loop() {
}
