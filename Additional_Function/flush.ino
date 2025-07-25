void flushBuffer(Stream &serialPort)
{while(serialPort.available()) byte r = serialPort.read();}
