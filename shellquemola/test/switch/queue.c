#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "queue.h"

k_addr peek() {
   return addr_array[front];
}

int isEmpty() {
   return itemCount == 0;
}

int isFull() {
   return itemCount == MAX;
}

int size() {
   return itemCount;
}  

void insert(k_addr data) {

   if(!isFull()) {
	
      if(rear == MAX-1) {
         rear = -1;            
      }       

      addr_array[++rear] = data;
      itemCount++;
   }
}

k_addr removeData() {
   k_addr data = addr_array[front++];
	
   if(front == MAX) {
      front = 0;
   }
	
   itemCount--;
   return data;  
}
