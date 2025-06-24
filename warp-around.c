#include <stdio.h>
#include <limits.h>

int main() 
{
    unsigned long st = 10;
    unsigned long en = 5;
    
    printf("a-b = %u\n", en-st);
    printf("ULONG_MAX - a + b = %u", ULONG_MAX-st+en+1);

    return 0;
}

# ---------------------------------------------------------- #
using System;

class Program
{
    static void Main()
    {
        uint start = 10;
        uint end = 5;

        Console.WriteLine($"End-start: {end-start}");
    }
}
