void sent_pos_get_protocols()
{
  byte pos_get[] = {0x02, 0x00};
  Serial2.write(pos_get, sizeof(pos_get));
  delay(100);
}

float* pos_get()
{ 
  sent_wake_up_protocols(); clear_protocol();
  sent_pos_get_protocols();

  while (!Serial2.available())
  {
    if(millis() - Time_out > 5000)
    {
      clear_protocol();
      int Time_out = millis();
      sent_pos_get_protocols();
    }
  }

  float* position_XYZ = (float*)malloc(3*sizeof(float));

  position_XYZ[0] = 0.0;
  position_XYZ[1] = 0.0;
  position_XYZ[2] = 0.0;

  byte idx = 0, TLV[20];
  for(int i=0;i<20;i++) TLV[i] = 0x00;

  do
  {
    byte res = Serial2.read();
    if(idx > 0 || res == 0x40)
    {
      TLV[idx] = res;
      idx++;
    }
  }while (Serial2.available() && idx < 20);


  if(valid_protocol(TLV))
  {
    /*for(int i=0;i<18;i++)
    {
      Serial.print(TLV[i], HEX);
      Serial.print(" ");
    }
    Serial.println();*/

    x = TLV[5]  | (TLV[6] << 8)  | (TLV[7] << 16)  | (TLV[8] << 24);
    y = TLV[9]  | (TLV[10] << 8) | (TLV[11] << 16) | (TLV[12] << 24);
    z = TLV[13] | (TLV[14] << 8) | (TLV[15] << 16) | (TLV[16] << 24);


    position_XYZ[0] = x/1000.0;
    position_XYZ[1] = y/1000.0;
    position_XYZ[2] = z/1000.0;
  }
  return position_XYZ;
}
