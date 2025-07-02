  byte len = Label.length();
  byte *cmd_label = (byte*)malloc((len+2)*sizeof(byte));

  // {0x1D, Label_Length, Label}
  cmd_label[0] = 0x1D;
  cmd_label[1] = len;

  for(byte i=0;i<len;i++)
    cmd_label[i+2] = int(Label.charAt(i));
