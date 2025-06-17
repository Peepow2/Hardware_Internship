int count_NumIndex_ByteArray(byte TYPE)
{
  if(TYPE == 0x00) sent_wake_up_protocols();
  if(TYPE == 0x02) sent_pos_get_protocols();
  if(TYPE == 0x0C) sent_loc_get_protocols();

  int N = 0;
  while (Serial2.available())
  {
    byte res = Serial2.read();
    N++;
  }
  return N;
}
