#include <stdio.h>
#include <stdlib.h>

#include "buffer.h"


buffer_item buffer[BUFFER_SIZE] = {0};

int in = 0;
int out = 0;
int count = 0;

int insert_item(buffer_item item){
 if(count == BUFFER_SIZE){
  fprintf(stderr, "Cannot add item to buffer, buffer is full\n");
  return -1;  
 }
 //printf("in %d\n", in);
 buffer[in] = item;
 in++;
 in = in % BUFFER_SIZE;
 count++;
 return 0; 
}

int remove_item(buffer_item* item){
 if(count == 0){
  fprintf(stderr, "Cannot remove item from buffer, buffer is empty\n");
  return -1;  
 }
  
 //printf("out %d\n", out);
 
 *item = getFirstInQueueItem(); 
 buffer[out] = 0; 
 out++;
 out = out % BUFFER_SIZE;
 count--;
 return 0; 
}   

int getFirstInQueueItem(){
 if(count == 0){
  perror("ERROR: buffer is empty, could not retrieve first item in queue\n");
  return -1;
 }
 return buffer[out];
}

int getBufferCount(){
 return count;
}

void printBufferContents(){
 int temp = 0;
 int i=0;
 printf("---------------------------------------------\n");
 printf("Buffer Contents\n");
 for(i=0;i<BUFFER_SIZE;i++){
  printf("buffer[%d]: %d\n", i, buffer[i]);
 }
 //printf("in %d out %d\n", in, out);
}
