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

//-----Func Definitions-----
int readFile(char filePath[], int array[]);
//-----End Definitions-----

int main(int argc, char* argv[])
{
  if (argc < 3 || argc > 3) //Not the correct number of cl args
  {
    printf ("The incorrect number of arguments were provided, expected 3 but %d were given.\n", argc);
    return -1;
  }
  
  //Read File and return the number of values parsed
  char* fPath = argv[1];
  int arraySize = 100000000;
  int* numArray = (int*)malloc(arraySize * sizeof(int));
  
  int numParsed = readFile(fPath, numArray);

  int threadsReq = atoi(argv[2]);
  if (threadsReq > numParsed)
  {
    printf("Too many threads requested!\n");
    free(numArray);
    return -1;
  }

  free(numArray);
  return 0;
}

int readFile(char filePath[], int array[])
{
  FILE* filePointer;
  filePointer = fopen(filePath, "r");

  if (filePointer == NULL)
  {
    printf("The file could not be opened!\n");
    return -1;
  }

  int valueRead, valuesParsed = 0;
  while (fscanf(filePointer, "%d", &valueRead) == 1)
  {
    array[valuesParsed++] = valueRead;
  }
  
  fclose(filePointer);
  return valuesParsed;
}
