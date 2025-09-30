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

typedef struct _thread_data_t {
  const int *data;
  int startInd;
  int endInd;
  pthread_mutex_t *lock;
  long long int *totalSum;
} thread_data_t;

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
  
  if (numArray == NULL)
  {
    printf("The memory allocation did not succeed!");
    return -1;
  }
  
  int numParsed = readFile(fPath, numArray);

  int threadsReq = atoi(argv[2]);
  if (threadsReq > numParsed)
  {
    printf("Too many threads requested!\n");
    free(numArray);
    return -1;
  }

  long long int totalSum = 0;

  //Initialize timer
  struct timeval {
    time_t tv_sec; //seconds
    suseconds tv_usec; //microseconds
  };
  int gettimeofday(&startTime, NULL);

  //Initialize pthread_mutex_t variable used by created threads to implement required locking
  int pthread_mutex_destroy(pthread_mutex_t *mutex);
  int pthread_mutex_init(pthread_mutex_t *restrict mutex, const pthread_mutexattr_t *restrict attr);
  pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

  //Construct array of thread_data_t objects as large as threadsReq
  for (int i = 0; i < threadsReq; i++)
  {
    //Loop through the array of thread_data_t and set the pointer to the data array containing the prev. read values
    //The startIndex and endIndex of the slice of the array for the Thread to process (i.e. to sum through)
    //Lock to point to the prev. created pthread_mutex_t
    //pthread_create and pass in the corresponding pthread_t object in the array, the routine to invoke arraysum, and the corresponding thread_data_t object
  }

  free(numArray);
  return 0;
}

void* arraySum
{
  long long int threadSum;
  /*
  It is assumed to operate on thread_data_t* input data (but since the input type is void* to
  adhere by the pthread API, you have to typecast the input pointer into the appropriate pointer
  type to reinterpret the data).
  It should sum the thread_data_t-&gt;data array from thread_data_t-&gt;startIndex to
  thread_data_t-&gt;endIndex (only a slice of the original array), into a locally defined long
  long int threadSum variable.
  Once it is done, it should update with its local sum the value stored in thread_data_t-
  &gt;totalSum.
  */

  return (void*)
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
