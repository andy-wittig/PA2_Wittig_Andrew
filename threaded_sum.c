//Andrew Wittig
//This threaded program will sum over an array from file and calculate the ms taken to fully execute the summation.

//-----Includes-----
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
#include <pthread.h>
#include <sys/time.h>
//-----End Includes-----

int main(int argc, char* argv[])
{
  if (argc < 3 || argc > 3) //Not the correct number of cl args
  {
    printf ("The incorrect number of arguments were provided, expected 3 but %d were given.\n", argc);
    return -1;
  }
  return 0;
}
