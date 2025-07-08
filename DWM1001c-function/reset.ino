byte cmd_reset[] = {0x14, 0x00};
bool reset()
{
  bool complete = false;
  byte* TLV_reset = send_cmd(cmd_reset);
  if(TLV_reset != NULL)
    complete = true;
  free(TLV_reset);
  return complete;
}
