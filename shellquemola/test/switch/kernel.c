#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "kernel.h"

#define MAX 2
#define ITERATIONS 1000

k_addr addr_array[MAX];
int front = 0;
int rear = -1;
int itemCount = 0;

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


void t1() {
	int i = ITERATIONS;
	while (i--) {
		kwrite("A");	
	}
}

void t2() {
	int i = ITERATIONS;
	while (i--) {
		kwrite("B");
	}
}

void kwrite(char * str) {
	int a[10] = {5, 5, 5, 5, 5, 5, 5, 5, 5, 5};
	k_addr adr;
	adr.esp = a[12];
	adr.offset = a[13];
	adr.addr = a[14];

	write(1, str, strlen(str));
	kyield(adr);
}

void jump_to(k_addr adr) {
	int a[10] = {5, 5, 5, 5, 5, 5, 5, 5, 5, 5};
	a[12] = adr.esp;
	a[13] = adr.offset;
	a[14] = adr.addr;
}

void kyield(k_addr from) {
	//getchar();
	k_addr jumper = sched(from);

	if (jumper.esp == from.esp && jumper.offset == from.offset && jumper.addr == from.addr) {
		printf("FIRST EXECUTION\n");
		t2();
	}

	jump_to(jumper);
	printf("ERROR\n");
}

k_addr sched(k_addr in) {
	k_addr tmp;
	if (isEmpty()) tmp = in; else tmp = removeData();
	insert(in);
	return tmp;
}

int main() {	
	//char t1_addr[20];
	//sprintf(t1_addr, "%p", t1);
	//memmove(t1_addr, t1_addr+6, strlen(t1_addr));
	//t1_a.addr = strtoul(t1_addr, NULL, 16);
	
	//char t2_addr[20];
	//sprintf(t2_addr, "%p", t2);
	//memmove(t2_addr, t2_addr+6, strlen(t2_addr));
	//t2_a.addr = strtoul(t2_addr, NULL, 16);

	func = 0;
	t1();

	return 0;
}
