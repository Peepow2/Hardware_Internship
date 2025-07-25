byte cmd_Check_Valid_response[] = {0x40, 0x01, 0x00};

byte* send_cmd(byte cmd[], Stream &serialPort)
{
  if(!connected(serialPort))
      return NULL;

  byte ByteArrSize = 150;
  byte *TLV_temp = (byte*)malloc(ByteArrSize * sizeof(byte));

  byte N = 5, TimeOUT = 5;
  while(N--)
  {
    byte index = 0;
    for(byte i=0;i<ByteArrSize;i++)  TLV_temp[i] = 0x00;

    byte sentcmd_Count = 0;
    flushBuffer(serialPort);
    
    while(sentcmd_Count <= TimeOUT && !serialPort.available())
    {
      serialPort.write(cmd, (cmd[1]+2)*sizeof(byte));
      sentcmd_Count++;
      delay(800);
    }

    if(sentcmd_Count > TimeOUT)
      continue;
      
    while (serialPort.available() && index < ByteArrSize)
    {
      byte rev = serialPort.read();
      if(index > 0 || rev == cmd_Check_Valid_response[0])
      {
        TLV_temp[index++] = rev;
      }
    }

    if(cmd_Check_Valid_response[0] == TLV_temp[0] \
        && cmd_Check_Valid_response[1] == TLV_temp[1] \
        && cmd_Check_Valid_response[2] == TLV_temp[2])
    {
      byte *TLV = (byte*)malloc(index * sizeof(byte));
      for(byte i=0;i<index;i++)
        TLV[i] = TLV_temp[i];
      free(TLV_temp);
      return TLV;
    }
  }
  free(TLV_temp);
  return NULL;
}
