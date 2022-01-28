#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#include "buffer.h"

unsigned int SEED = 1234;
const int MAX_MSECONDS = 1000;
const int MAX_ITEM_VALUE = 500; 

int itemsProduced = 0;
int maxItemsAllowed = 0;

int aliveCountProducer = 0;
int aliveCountConsumer = 0;

int isMaxItemsProduced = 0;
int isExitAllThreads = 0;

sem_t full;
sem_t empty;
pthread_mutex_t mutex;

int generateRandomInt(){
 return rand_r(&SEED); 
}

void *producerRunner(void *param){
 //if empty then proceed else block
 //if mutex then proceed and lock mutexelse block
 //when done signal full and unlock mutex
 buffer_item item = 0;
 const int id = *((int*)param);
 int randInt = 0;
 int i;
 int isRun = 1;
 //printf("prod id %d\n",id);
 while(isRun){
  randInt = (generateRandomInt() % MAX_MSECONDS) + 1;
  usleep((unsigned int) randInt);

  sem_wait(&empty);
  pthread_mutex_lock(&mutex);
  
  if(isMaxItemsProduced == 1){
   isRun = 0; 
   //printf("producer alive %d\n", aliveCountProducer); 
   //Keep on signaling consumer to run so that all consumer can exit
   if(aliveCountProducer == 1 && aliveCountConsumer > 1){
    isRun = 1;
    //printf("consumer alive %d\n", aliveCountConsumer); 
   }
   else{
    aliveCountProducer--;
   }
  }
  else{
   item = 1 + (randInt % (MAX_ITEM_VALUE-1));
   if(insert_item(item) == 0){
    printf("Producer %d 	produced %d\n", id, item);
    itemsProduced++;
    if(itemsProduced == maxItemsAllowed){
     isMaxItemsProduced = 1;
     printf("max items produced flag set\n");
    }
   }
  }
  // printf("Locked\n");	
  
  sem_post(&full);
  pthread_mutex_unlock(&mutex);                                            
  
  //printf("Unlocked\n");
 }
 printf("Producer %d 	Exiting........\n", id);
}

void *consumerRunner(void *param){
 //if full then proceed else block
 //if mutex then proceed else block
 //when done signal empty and unlock mutex
 const int id = *((int*)param); 
 int randInt = 0;
 int i;
 int isRun = 1;
 //printf("con id %d\n",id);
 //int emptySemValue = 0;
 buffer_item rmItem = -1;
 while(isRun){
  randInt = generateRandomInt() % MAX_MSECONDS;
  usleep((unsigned int) randInt);
  
  sem_wait(&full);
  pthread_mutex_lock(&mutex);
  
  if(isExitAllThreads == 1){
   isRun = 0; 
   if(aliveCountProducer > 1 && aliveCountConsumer == 1){
    isRun = 1; 
   }
   else{
    aliveCountConsumer--;
   }
  } 
  else if(remove_item(&rmItem) != -1){ 
   printf("Consumer %d 	consumed %d\n", id, rmItem);
   if(isMaxItemsProduced == 1 && getBufferCount() == 0){
    isExitAllThreads = 1;
    printf("exit flag set\n");
   }
  }
  // printf("Locked\n"); 
  
  sem_post(&empty);
  pthread_mutex_unlock(&mutex);
  //printf("Unlocked\n");
 }
 printf("Consumer %d 	Exiting........\n", id);
 
}


int main(int argc, char* argv[]){
 //printf("adfasdfzzzzzzzzzzzz\n");
 void initSemaphoresAndLocks(int maxItems){
  sem_init(&full,0, 0);
  sem_init(&empty,0, maxItems);
  pthread_mutex_init(&mutex, NULL);
 }

 void createThreads(pthread_t* threadArr, int size, pthread_attr_t* attr, void*(*fn)(void*), int* numArr){
  numArr = calloc(size, sizeof(int)); 
  int i;
  for(i=0;i<size;i++){
   numArr[i] = i;
   if(pthread_create(&threadArr[i], attr, fn, &numArr[i])!=0){
    perror("ERROR: pthread_create() failed\n");
    exit(1);
   }
  //printf("thread id %d\n", (int)threadArr[i]);
  }
 }

 int checkParams(){
  int isError = 0;
  if(argc != 4){
   perror("ERROR: main requires 3 paramaters\n");
   isError = 1;
  }
  else if(atoi(argv[1]) == 0 || atoi(argv[2]) == 0 || atoi(argv[3]) == 0){
   perror("ERROR: paramaters for main must be greater than 0\n");
   isError = 1;
  }

  if(isError == 1) exit(-1);  
 }
  
 checkParams();
 const int numberOfProducer = atoi(argv[1]);
 const int numberOfConsumer = atoi(argv[2]);
 const int maxItemsProduced = atoi(argv[3]);

 aliveCountProducer = numberOfProducer;
 aliveCountConsumer = numberOfConsumer;
 maxItemsAllowed = maxItemsProduced;

 initSemaphoresAndLocks(maxItemsProduced);

 pthread_t prodThreadArr[numberOfProducer];
 pthread_t conThreadArr[numberOfConsumer];
 pthread_attr_t threadAttr;
 pthread_attr_init(&threadAttr); 

 int* prodNumArr;
 int* conNumArr;
 createThreads(prodThreadArr, numberOfProducer, &threadAttr, producerRunner, prodNumArr);
 createThreads(conThreadArr, numberOfConsumer, &threadAttr, consumerRunner, conNumArr);

 int emptySemValue = 0;
 int i;
 for(i=0;i<numberOfProducer;i++){
  pthread_join(prodThreadArr[i], NULL); 
 }
 printf("All Producer threads exited\n");

 for(i=0;i<numberOfConsumer;i++){
  pthread_join(conThreadArr[i], NULL); 
  //printf("con i %d\n", i); 
 }
 printf("All Consumer threads exited\n");
 
 printBufferContents();
 free(prodNumArr);
 free(conNumArr);
 printf("End of program\n");
}
