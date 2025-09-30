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

//Thread structure
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
  //time_t tv_sec - seconds
  //suseconds tv_usec - microseconds
  struct timeval startTime, endTime;
  gettimeofday(&startTime, NULL);

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

void* arraySum(void* threadData)
{
  long long int threadSum;
  thread_data_t* tData = (thread_data_t*)threadData; //Reinterpret the data to adhere by the pthread API

  for (int i = tData->startInd; i < tData->endInd; i++) //Get local sum from the "chunk" of data from the Thread
  {
    threadSum += tData->data[i];
  }

  //Add local sum to the total sum
  pthread_mutex_lock(tData->lock);
  *(tData->totalSum) += threadSum;
  pthread_mutex_unlock(tData->lock);
  pthread_exit(NULL);
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
