#pragma once
#include "k_types.h"

#define MAX 2

k_addr addr_array[MAX];
int front = 0;
int rear = -1;
int itemCount = 0;

k_addr peek();
int isEmpty();
int isFull();
int size();
void insert(k_addr);
k_addr removeData();
