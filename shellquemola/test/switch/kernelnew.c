#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "kernel.h"

#define MAX 2
#define ITERATIONS 1000

int init1;
int init2;
int init3;

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

p_list head;
void init_proc_list() {
	head = malloc(sizeof(p_list));
	(head->adr).esp=-1;
	(head->adr).offset=-1;
	(head->adr).addr=-1;
	head->next = NULL;
}

void insert_proc(k_addr in) {
	p_list actual = head;
	while (actual->next != NULL) actual = actual->next;
	p_list node = malloc(sizeof(p_list));
	node->adr = in;
	node->next = NULL;
	actual->next = node;
}

int proc_number() {
	p_list actual = head;
	int i = 0;
	while(actual->next != NULL) {i++; actual = actual->next;}
	return i;
}

int search_proc(k_addr search) {
	p_list actual = head;
	while (actual->next != NULL) {
		if((actual->adr).esp == search.esp && (actual->adr).offset == search.offset && (actual->adr).addr == search.addr) {
			return 1;
		}
		actual = actual->next;
	}
	return 0;
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

	init1 = 1;	
	init2 = 1;
	init3 = 1;

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

	if (init1) {
		printf("\nFIRST EX\n");
		init1=0;
		t1();	
	}
	if (init2) {
		printf("\nSECOND EX\n");
		init2=0;			
		t2(); 
	}
	if (init3) {
		printf("\nTHIRD EX\n");
		init3=0;			
		t3();
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

	init_kernel();

	return 0;
}
