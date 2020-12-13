/*
 *
 *    Created by David Pratt
 *
 *
 *    For this project, I created an example program that creates
 *    two threads. Those two threads then attempt to increment a variable
 *    called resource. The program initializes two booleans to track 
 *    weather or not the given thread can access the variable. When one is
 *    is accessing the resource, that threads boolean is set to true to allow
 *    and the other's is set to false to block it. When it begins accessing the
 *    resource, a timer is started. When the timer runs out, the booleans are
 *    switched so that the other thread can begin accessing the resource.
 *
*/


#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>


int resource = 0;
int timerIsRunning = 0;
/*set resource as free*/
volatile int isAccessibleFromThread1 = 1;
volatile int isAccessibleFromThread2 = 0;

/*used to change resource to locked*/
void sigalrm_handler(int sig) 
{
  /*
  **  Lock current threads access to resource
  */
  if (isAccessibleFromThread1) 
  {
    isAccessibleFromThread1 = 0;
    isAccessibleFromThread2 = 1;
  } 
  else if (isAccessibleFromThread2) 
  {
    isAccessibleFromThread2 = 0;
    isAccessibleFromThread1 = 1;
  }
}

void startTimer_AccessingResource() 
{

  //set timer to running
  timerIsRunning = 1;
  //create sigaction struct
  struct sigaction sact;
  //set signal set
  sigemptyset(&sact.sa_mask);
  //set sigaction flags to nothing
  sact.sa_flags = 0;
  //set signal alarm handler to sigalrm_handler
  sact.sa_handler = sigalrm_handler;
  //set signal action
  sigaction(SIGALRM, &sact, NULL);
  //get alarm in 20 seconds to free resource
  alarm(20);
  
  /*while (IsAccessible) 
  {
     printf("Resource is accessible\n");
     sleep(2);
  }
  
  printf("Resource Locked\n"); */
}

void* thread1_function() 
{
  while (1) 
  {
    if (isAccessibleFromThread1) 
    {
      if (!timerIsRunning) 
      {
        startTimer_AccessingResource();
      }
      
      printf("Thread 1 Accessing Resource; Resource = %d\n", resource);
      resource++;
      sleep(2);
    }
    else 
    {
      printf("Thread 1 Blocked\n");
      sleep(2);
    } 
  }  
}


void* thread2_function()
{
  while (1) 
  {
    if (isAccessibleFromThread2) 
    {
      if (!timerIsRunning) 
      {
        startTimer_AccessingResource();
      }
      
      printf("Thread 2 Accessing Resource; Resource = %d\n", resource);
      resource++;
      sleep(2);
    }
    else 
    {
      printf("Thread 2 Blocked\n");
      sleep(2);
    } 
  }
}

int main() 
{
  pthread_t thread1, thread2;
  
  pthread_create(&thread1, NULL, thread1_function, NULL);
  pthread_create(&thread2, NULL, thread2_function, NULL);
  
  pthread_join(thread1, NULL);
  sleep(1);
  pthread_join(thread2, NULL);
  
  exit(0);
}
