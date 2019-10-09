#pragma once
#include "k_types.h"

#define MAX 4
#define ITERATIONS 1000

typedef struct {
	void * addr;
	int flag;
} procs;

procs processes[MAX-1];

int func;

void kyield(k_addr);
void kwrite(char *);
k_addr sched(k_addr);
