#define RX 16
#define TX 17

void sendCommand(String cmd) 
{
  Serial1.print(cmd + "\r");
  delay(200);
  while (Serial1.available()) 
  {
    char c = Serial1.read();
    Serial.print(c);
  }
}

void setup() 
{
  Serial.begin(115200);
  Serial1.begin(115200, SERIAL_8N1, RX, TX); // RX=GPIO16, TX=GPIO17

  Serial.println("ตั้งค่า DWM1001C เป็น TAG");

  sendCommand("nmt");   // ตั้งเป็น TAG
  sendCommand("lec");   // เปิด Location Engine
  sendCommand("lep");   // เปิด Position Output
  sendCommand("reset"); // รีเซ็ตให้ค่าเริ่มทำงาน
  delay(3000);          // รอโมดูลบู๊ตใหม่
}

void loop() 
{
  if (Serial1.available()) 
  {
    String line = Serial1.readStringUntil('\n');
    Serial.println("[ตำแหน่ง] " + line);
  }
  delay(100);
}
