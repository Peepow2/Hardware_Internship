#include <HardwareSerial.h>

HardwareSerial dwmSerial(1);  // UART1

void sendCommand(String cmd) {
  dwmSerial.print(cmd + "\r");
  delay(200);
  while (dwmSerial.available()) {
    char c = dwmSerial.read();
    Serial.print(c);
  }
}

void setup() {
  Serial.begin(115200);
  dwmSerial.begin(115200, SERIAL_8N1, 16, 17); // RX=GPIO16, TX=GPIO17

  delay(2000);  // รอให้โมดูลพร้อม

  Serial.println("ตั้งค่า DWM1001C เป็น ANCHOR");

  sendCommand("nma");   // ตั้งเป็น ANCHOR
  sendCommand("apg");   // เปิด Auto Positioning (Anchor จะรู้ตำแหน่งตัวเอง)
  sendCommand("reset"); // รีเซ็ตเพื่อเริ่มทำงาน
  delay(3000);          // รอให้โมดูลบู๊ตใหม่
}

void loop() {
  // ไม่มีการดึงข้อมูลจาก Anchor
}
