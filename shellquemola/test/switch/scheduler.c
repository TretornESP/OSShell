#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "kernel.h"

#define MAX 3
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

proc *proc_array[MAX];
int pfront = 0;
int prear = -1;
int procCount = 0;

proc *proc_peek() {
   return proc_array[front];
}

int proc_isEmpty() {
   return procCount == 0;
}

int proc_isFull() {
   return procCount == MAX;
}

int proc_size() {
   return procCount;
}  

void proc_insert(proc *data) {

   if(!proc_isFull()) {
	
      if(prear == MAX-1) {
         prear = -1;            
      }       

      proc_array[++prear] = data;
      procCount++;
   }
}

proc *proc_removeData() {
   proc *data = proc_array[pfront++];
	
   if(pfront == MAX) {
      pfront = 0;
   }
	
   procCount--;
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

void t3() {
	int i = ITERATIONS;
	while (i--) {
		kwrite("C");
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

void init_kernel() {
	proc *p_t1 = malloc(sizeof(proc));
	strcpy(p_t1->name, "T1\0");
	p_t1->ptr = &t1;
	proc_insert(p_t1);

	proc *p_t2 = malloc(sizeof(proc));
	strcpy(p_t2->name, "T2\0");
	p_t2->ptr = &t2;
	proc_insert(p_t2);

	proc *p_t3 = malloc(sizeof(proc));
	strcpy(p_t3->name, "T3\0");
	p_t3->ptr = &t3;
	proc_insert(p_t3);

	printf("EN COLA %d\n", proc_size());


	int a[10] = {5, 5, 5, 5, 5, 5, 5, 5, 5, 5};
	k_addr adr;
	adr.esp = a[12];
	adr.offset = a[13];
	adr.addr = a[14];
	
	kyield(adr);
}


void kyield(k_addr from) {
	//getchar();
	k_addr jumper = sched(from);
	jump_to(jumper);
	printf("ERROR\n");
}

k_addr sched(k_addr in) {
	k_addr tmp;
	if (isEmpty()) tmp = in; else tmp = removeData();
	insert(in);

	if (!proc_isEmpty()) {
		proc *p = proc_removeData();
		printf("\nSALTAMOS A %s %d EN COLA\n", p->name, proc_size());
		void (*ptr)() = p->ptr;
		ptr();	
	}

	return tmp;
}

int main() {

	init_kernel();

	return 0;
}
