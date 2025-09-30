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
void* arraySum(void* threadData);
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
  const int MAX_INPUT_SIZE = 100000000;
  int* numArray = (int*)malloc(MAX_INPUT_SIZE * sizeof(int));
  
  if (numArray == NULL)
  {
    printf("The memory allocation for numArray did not succeed!");
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
  pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
  
  //Dynamically Allocate thread objects as large as threadsReq
  pthread_t* threads = malloc(sizeof(pthread_t) * threadsReq);
  thread_data_t* mThread = malloc(sizeof(thread_data_t) * threadsReq);

  if (threads == NULL || mThread == NULL)
  {
    printf("The memory allocation for threads did not succeed!");
    free(numArray);
    return -1;
  }

  //Seperate work for threads
  int threadsSplit = numParsed / threadsReq;
  int threadsRemaining = numParsed % threadsReq;
  int currentThreadInd = 0;
  
  for (int i = 0; i < threadsReq; i++)
  {
    //Loop through the array of thread_data_t and set the pointer to the data array containing the prev. read values
    //The startIndex and endIndex of the slice of the array for the Thread to process (i.e. to sum through)
    //Lock to point to the prev. created pthread_mutex_t
    //pthread_create and pass in the corresponding pthread_t object in the array, the routine to invoke arraysum, and the corresponding thread_data_t object
    
    int extra = (i < threadsRemaining) ? 1 : 0;
    int endThreadInd = currentThreadInd + threadsSplit + extra;
    
    mThread[i].data = numArray;
    mThread[i].startInd = currentThreadInd;
    mThread[i].endInd = endThreadInd;
    mThread[i].lock = &mutex;
    mThread[i].totalSum = &totalSum;
    currentThreadInd = endThreadInd;

    pthread_create(&threads[i], NULL, arraySum, &mThread[i]);
  }

  //Allow threads to complete
  for (int i = 0; i < threadsReq; i++)
  {
    pthread_join(threads[i], NULL);
  }
  //Get end time
  gettimeofday(&endTime, NULL);
  long long timeSpentInMilliSeconds = (endTime.tv_sec - startTime.tv_sec) * 1000LL + (endTime.tv_usec - startTime.tv_usec) / 1000;

  printf("Total sum is: %lld\n", totalSum);
  printf("Time elapsed was %lld milliseconds\n", timeSpentInMilliSeconds);

  //Memory cleanup
  free(numArray);
  free(threads);
  free(mThread);
  pthread_mutex_destroy(&mutex);
  
  return 0;
}

void* arraySum(void* threadData)
{
  long long int threadSum = 0;
  thread_data_t* tData = (thread_data_t*)threadData; //Reinterpret the data to adhere by the pthread API

  for (int i = tData->startInd; i < tData->endInd; i++) //Get local sum from the "chunk" of data from the Thread
  {
    threadSum += tData->data[i];
  }

  //Add local sum to the total sum ensuring thread saftey
  pthread_mutex_lock(tData->lock); //Lock and unlock the Critical Section
  *(tData->totalSum) += threadSum; //Update total sum in thread_data_t
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
