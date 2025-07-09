byte cmd_reset[] = {0x14, 0x00};
byte cmd_factory_reset[] = {0x13, 0x00};

bool reset()
{
  byte* TLV = send_cmd(cmd_reset);
  if(TLV == NULL) return false;
  free(TLV);
  return true;
}

bool factory_reset()
{
  byte* TLV = send_cmd(cmd_factory_reset);
  if(TLV == NULL) return false;
  free(TLV);
  return true;
}
