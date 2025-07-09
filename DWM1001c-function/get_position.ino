byte cmd_getposTAG[] = {0x02, 0x00};

float* get_pos()
{
  byte* TLV = send_cmd(cmd_getposTAG);
  
  float* POS = (float*)malloc(3*sizeof(float));
  POS[0] = (TLV[5]  |  TLV[6] << 8 |  TLV[7] << 16 |  TLV[8] << 24) / 1000.0;
  POS[1] = (TLV[9]  | TLV[10] << 8 | TLV[11] << 16 | TLV[12] << 24) / 1000.0;
  POS[2] = (TLV[13] | TLV[14] << 8 | TLV[15] << 16 | TLV[16] << 24) / 1000.0;

  free(TLV);
  return POS;
}
