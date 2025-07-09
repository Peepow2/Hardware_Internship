byte cmd_get_config[] = {0x08, 0x00};

void get_config()
{
  byte *TLV = send_cmd(cmd_get_config);
  if(TLV == NULL) return;
  for(byte i=5;i<7;i++)
  {
    Serial.print(TLV[i], BIN);
    Serial.print(" ");
  }Serial.println("\n");
  
  Serial.print("BYTE0\n");
  byte BYTE0 = TLV[5];

  Serial.print("uwb_mode: "); 
  Serial.println(BYTE0 & 0b11, BIN);

  Serial.print("fw_update_en: "); 
  Serial.println(BYTE0 >> 2 & 1);

  Serial.print("ble_en: "); 
  Serial.println(BYTE0 >> 3 & 1);

  Serial.print("led_en: "); 
  Serial.println(BYTE0 >> 4 & 1);

  Serial.print("enc_en: "); 
  Serial.println(BYTE0 >> 5 & 1);

  Serial.print("loc_engine_en: "); 
  Serial.println(BYTE0 >> 6 & 1);

  Serial.print("low_power_en: "); 
  Serial.println(BYTE0 >> 7 & 1);

  free(TLV);
}
