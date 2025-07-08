byte* send_cmd(byte cmd[])
{
  byte ByteArrSize = 110;
  byte *TLV = (byte*)malloc(ByteArrSize * sizeof(byte));

  byte N = 5, TimeOUT = 5;
  while(N--)
  {
    byte index = 0;
    for(byte i=0;i<ByteArrSize;i++)  TLV[i] = 0x00;

    byte sentcmd_Count = 0;
    Serial2.flush();

    while(sentcmd_Count <= TimeOUT && !Serial2.available())
    {
      Serial2.write(cmd, (cmd[1]+2)*sizeof(byte));
      sentcmd_Count++;
      delay(1000);
    }

    if(sentcmd_Count > TimeOUT && !Serial2.available())
      continue;

    while (Serial2.available() && index < ByteArrSize)
    {
      byte rev = Serial2.read();
      if(index > 0 || rev == cmd_Check_Valid_response[0])
      {
        TLV[index++] = rev;
      }
    }

    if(cmd_Check_Valid_response[0] == TLV[0] \
        && cmd_Check_Valid_response[1] == TLV[1] \
        && cmd_Check_Valid_response[2] == TLV[2])
    { 
      return TLV;
    }
  }
  free(TLV);
  return NULL;
}
