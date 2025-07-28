bool grid[] = {1,1,1,1,1,1,1,1,1,1,\
               1,1,0,0,0,0,0,0,0,0,\
               1,0,1,1,0,1,1,0,1,1,\
               1,0,1,1,0,1,1,0,1,1,\
               1,0,1,1,0,1,1,0,1,1,\
               1,0,1,1,0,1,1,0,1,1,\
               1,0,1,1,0,1,1,0,1,1,\
               1,0,1,1,0,1,1,0,1,1,\
               1,0,1,1,0,0,0,0,1,1,\
               1,0,1,1,0,1,1,0,1,1,\
               1,0,1,1,0,1,1,0,1,1,\
               1,0,1,1,0,1,1,0,1,1,\
               1,0,0,0,0,0,0,0,0,1,\
               0,0,1,1,1,1,1,1,1,1};

char walk[] = {1,1,1,1,1,1,1,1,1,1,\
               1,1,0,0,0,0,0,0,0,0,\
               1,0,1,1,0,1,1,0,1,1,\
               1,0,1,1,0,1,1,0,1,1,\
               1,0,1,1,0,1,1,0,1,1,\
               1,0,1,1,0,1,1,0,1,1,\
               1,0,1,1,0,1,1,0,1,1,\
               1,0,1,1,0,1,1,0,1,1,\
               1,0,1,1,0,0,0,0,1,1,\
               1,0,1,1,0,1,1,0,1,1,\
               1,0,1,1,0,1,1,0,1,1,\
               1,0,1,1,0,1,1,0,1,1,\
               1,0,0,0,0,0,0,0,0,1,\
               0,0,1,1,1,1,1,1,1,1};

struct PathInfo
{
  String Path = "";
  int to = 0, dist = 0;
};

int Shortest_path()
{
  bool visited[140];
  for(byte i=0;i<140;i++) visited[i] = 0;
  
  PathInfo Queue[100];
  int len = 0, idx = 0;
  
  Queue[len].Path = "";
  Queue[len].to = 130;
  Queue[len].dist = 0;
  len = (len+1)%100;

  String P = "";
  while(len != idx)
  {
    int u = Queue[idx].to;
    int d = Queue[idx].dist;
    P = Queue[idx].Path;
    idx = (idx+1)%100;

    if(u==19)
    {
      Serial.println(P);
      break;
    }

    if (!visited[u])
    {
      visited[u] = 1;
      if (u+10<140 && grid[u+10]!=1)
      {
        Queue[len].to = u+10;
        Queue[len].dist = d+1;
        Queue[len].Path = P + 'D';
        len = (len+1)%100;        
      }

      if (u-10>=0 && grid[u-10]!=1)
      {
        Queue[len].to = u-10;
        Queue[len].dist = d+1;
        Queue[len].Path = P + 'U';
        len = (len+1)%100;     
      }

      if (u%10!=9 && grid[u+1]!=1)
      {
        Queue[len].to = u+1;
        Queue[len].dist = d+1;
        Queue[len].Path = P + 'R';
        len = (len+1)%100;           
      }

      if (u%10!=0 && grid[u-1]!=1)
      {
        Queue[len].to = u-1;
        Queue[len].dist = d+1;
        Queue[len].Path = P + 'L';
        len = (len+1)%100;             
      }      
    }
  }

  int LEN = P.length();
  int x = 130;
  int Array_path[25];
  len = 0;

  walk[x] = '*';
  for(byte i=0;i<LEN;i++)
  {
    char c = P.charAt(i);
    if (c == 'L') x -= 1;
    if (c == 'R') x += 1;
    if (c == 'U') x -= 10;
    if (c == 'D') x += 10;
    Array_path[len++] = x;
  }
  for(byte i=0;i<LEN;i++)
    walk[Array_path[i]] = '*';

  for(byte i=0;i<14;i++)
  {
    for(byte j=0;j<10;j++)
    {
      if(walk[i*10+j] != '*')
        Serial.print(walk[i*10+j]+0);
      else
        Serial.print('*');
      Serial.print(" ");
    }Serial.println();
  }
  return P.length();
}

void setup() 
{
  Serial.begin(115200);
}

void loop() 
{
  /*for(byte i=0;i<5;i++)
  {
    for(byte j=0;j<5;j++)
    {
      Serial.print(grid[i*5+j]);
      Serial.print(" ");
    }Serial.println();
  }Serial.println("\n");*/

  int SHP = Shortest_path();
  Serial.println(SHP);
  delay(5000);
}
