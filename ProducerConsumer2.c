#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/mman.h>
#include <errno.h>

#define MAX_TIMES 5

//Constans
int MAX = 100;
int WAKEUP = SIGUSR1;
int SLEEP = SIGUSR2;

//the child PID if the Parent else the Parent if the Child
pid_t otherPid;

//A signal set
sigset_t sigSet;

//Shared Circular Buffer
struct CIRCULAR_BUFFER
{
  int count;
  int lower;
  int upper;
  int buffer[100];
};
struct CIRCULAR_BUFFER *buffer = NULL;




/*****************************************************/





//method to put the current Process to sleep forever until awoken by WAKEUP
void sleepAndWait() {
  int nSig;
  
  printf("Sleeping honk shooo......\n");
  sigwait(&sigSet, &nSig);
  printf("Awoken\n");
}





//method to signal other process to WAKEUP
void wakeupOther() {
  kill(otherPid, WAKEUP);
}






//method to get value from shared buffer
int getValue() {


  if (buffer->count == 25) 
  {
    printf("Sleeping Consumer");
    sleepAndWait();
    return -1;  
  }
  else 
  {
    int returnVal = buffer->buffer[buffer->lower];
    printf("returVal = %d\n", returnVal);
   
    printf("buffer->lower = %d\n", buffer->lower);
    printf("buffer[lower] = %d\n", buffer->buffer[buffer->lower]);
    printf("buffer->count = %d\n", buffer->count);
  
  
    buffer->buffer[buffer->lower] = 0;
    buffer->lower++;
    buffer->count--;

    return returnVal;
  } 
  

/*    
  printf("buffer->lower = %d\n", buffer->lower);
  printf("buffer[lower] = %d\n", buffer->buffer[buffer->lower]);
  printf("buffer->count = %d\n", buffer->count);

  return returnVal;
*/
}






//method to put value into buffer
void putValue(struct CIRCULAR_BUFFER* buffer, int value) {
  if (buffer->upper == MAX) 
  {
    sleepAndWait();
  }
  else 
  {
    buffer->buffer[buffer->upper] = value;
    buffer->upper++;
    buffer->count++;
  }
}





/**********************************************************/




/*
**Consumer Logic
*/
void consumer() {
  int consume;
  
  sigemptyset(&sigSet);
  sigaddset(&sigSet, WAKEUP);
  
  //Run Consumer
  printf("Running Consumer\n");
  
  while (1) {
    //consume buffer[lower]
    int val = getValue();
    printf("%d\n", val);
        
    //test if buffer is empty
    if (buffer->count <= 1) {
      wakeupOther();
      printf("Sleeping Consumer\n");
      sleepAndWait();      
    }
  }
}





/*
** Producer Logic
*/
void producer() {
  //value to add to buffer
  int value = 0;
  
  //Set signal Set to watch for WAKEUP signal
  sigemptyset(&sigSet);
  sigaddset(&sigSet, WAKEUP);
  
  //producer logic
  printf("Running Producer\n");
  
  while (1) {
    putValue(buffer, value);
    value++;

    //test if buffer is full
    if (buffer->count == 99) {
      wakeupOther();
      sleepAndWait();
    }
    
  }
}






int main(int argc, char* argv[]) {

  pid_t pid;

  buffer = (struct CIRCULAR_BUFFER*)mmap(0, sizeof(buffer), PROT_READ |  PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
  buffer->count = 0;
  buffer->lower = 0;
  buffer->upper = 1;
  
  //set both processes tasks
  printf("************************BEFORE******************\n");
  printf("Buffer->upper = %d\n", buffer->upper);
  printf("Buffer->lower = %d\n", buffer->lower);
  printf("Buffer->count = %d\n", buffer->count);
  
  pid = fork();
  
  //test if fork happened correctly
  if (pid == -1) 
  {
    printf("Error forking...Error: %d\n", errno);
    exit(EXIT_FAILURE);
  }

  if (pid == 0 ) 
  {
    otherPid = pid;
    consumer();
  } 
  else 
  {
    otherPid = getppid();
    producer();
  }
  
  sleep(50);
  //return OK
  return 0;
}
















