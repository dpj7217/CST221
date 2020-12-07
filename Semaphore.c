#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

sem_t mutex;

/*Helper method to create thread*/
void* thread() {
  printf("Beginning");
  
  sleep(4);
  
  printf("Ending");
  
  return NULL;
}

int main() {
  //init semaphore
  sem_init(&mutex, 0, 1);
  
  //init 2 threads
  pthread_t t1, t2;
  
  //create 2 threads
  pthread_create(&t1, NULL, thread, NULL);
  pthread_create(&t2, NULL, thread, NULL);
  
  //join threads together
  pthread_join(t1, NULL);
  pthread_join(t2, NULL);
  
  //destroy semaphore
  sem_destroy(&mutex);
  return 0;
  
}
