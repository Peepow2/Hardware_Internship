#include <HardwareSerial.h>

HardwareSerial dwmSerial(1);  // ใช้ UART1

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

  delay(2000); // รอโมดูลพร้อม

  Serial.println("ตั้งค่า DWM1001C เป็น TAG");

  sendCommand("nmt");   // ตั้งเป็น TAG
  sendCommand("lec");   // เปิด Location Engine
  sendCommand("lep");   // เปิด Position Output
  sendCommand("reset"); // รีเซ็ตให้ค่าเริ่มทำงาน
  delay(3000);          // รอโมดูลบู๊ตใหม่
}

void loop() {
  if (dwmSerial.available()) {
    String line = dwmSerial.readStringUntil('\n');
    Serial.println("[ตำแหน่ง] " + line);
  }

  delay(100);
}
