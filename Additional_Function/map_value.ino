// Already exist as map
int map_value(float value, float A, float B, float C, float D)
{return (A!=B)?int((value-A)*(D-C)/(B-A)+C):A;}
