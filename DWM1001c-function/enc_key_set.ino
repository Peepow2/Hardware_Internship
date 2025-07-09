byte min(byte a, byte b)
{return (a < b) ? a : b;}

bool All_Zero(String S)
{
  byte len = min(S.length(), 16);
  for(byte i=0;i<len;i++)
  {
    if(S.charAt(i) != '0')
      return false;
  }
  return true;
}

byte head_cmd_enc_key_set[] = {0x3C, 0x16};

bool enc_key_set(String enc_key)
{
  if(All_Zero(enc_key))
  {
    Serial.println("Invalid Key");
    return false;
  }

  byte* cmd_enc_key_set = (byte*)malloc(16*sizeof(byte));
  for(byte i=0;i<16;i++) cmd_enc_key_set[i] = 0x00;

  cmd_enc_key_set[0] = head_cmd_enc_key_set[0];
  cmd_enc_key_set[1] = head_cmd_enc_key_set[1];

  byte len = min(enc_key.length(), 16);
  for(byte i=0;i<len;i++)
  {
    cmd_enc_key_set[i+2] = enc_key.charAt(i);
  }

  byte* TLV = send_cmd(cmd_enc_key_set);
  if(TLV == NULL) 
    return false;
  free(TLV);
  return true;
}
