void sent_loc_get_protocols()
{
  byte pos_get[] = {0x0C, 0x00};
  Serial2.write(pos_get, sizeof(pos_get));
  delay(100);
}

float* loc_get()
{
  return position_distance_of_anchor;
}
