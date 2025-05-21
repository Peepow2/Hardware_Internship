#define Rx 16
#define Tx 17

int Check_sum(int CS, int data)
{
  return CS ^ data;
}

int base10(char N)
{
  String BASE10 = "0123456789ABCDEF";
  for(char i=0;i<16;i++)
  {
    if(BASE10[i] == N)
      return i;
  }
  return -1;
}

bool Protocol_Check(String mess)
{
  char Mess_length = mess.length(), idx = 0;
  char Num_BS = 0; 

  for(char i=0;i<Mess_length;i++)
  {
    if(mess[i] == '\\')
      Num_BS++;
  }
  //Serial.println(Num_BS);
  if(Num_BS < 7) return false;

  String frame[Num_BS], S = "0";
  for(char i=3;i<Mess_length;i++)
  {
    if(mess[i] == '\\' || mess[i] == '\'')
    {
      frame[idx] = S;
      idx++;
      S = "0";
    }
    else
    {
      S += mess[i];
    }
  }
  
  if (frame[0] != "0x02" or frame[Num_BS-1] != "0x04")
    return false;
  
  int check_sum = 0x00;
  for(char i=1;i<Num_BS-2;i++)
    check_sum = Check_sum(check_sum, base10(frame[i][2]) * 16 +  base10(frame[i][3]));

  if(check_sum != base10(frame[Num_BS-2][2]) * 16 +  base10(frame[Num_BS-2][3]))
    return false;
  
  /*for(int i=0;i<Num_BS;i++)
  {
    Serial.print(i);
    Serial.print(" --> ");
    Serial.println(frame[i]);
  }*/
  return true;
}
