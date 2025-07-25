void flushBuffer(Stream &serialPort)
{while(serialPort.available()) byte r = serialPort.read();}

bool connected(Stream &serialPort)
{
  bool connected_SerialPort = false;

  byte test[] = {0x00}; 
  serialPort.write(test, sizeof(test)); delay(100);

  if(serialPort.available())
  {
    byte rev = serialPort.read();
    while(serialPort.available() && rev != 0x40)
      { rev = serialPort.read();}

    connected_SerialPort = true;
    for(byte i=0;i<2;i++)
    {
      if(!serialPort.available() || serialPort.read() != 0x01)
      { connected_SerialPort = false;}
    }
  }
  flushBuffer(serialPort);
  return connected_SerialPort;
}
