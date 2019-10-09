#pragma once
#define P_NAME_MAX 200

struct addr {
	unsigned int esp;
	unsigned int offset;
	unsigned int addr;
} addr;
typedef struct addr k_addr;

struct proc_list {
	k_addr adr;
	struct proc_list* next;
} proc_list;
typedef struct proc_list *p_list;

struct _proc {
	char name[P_NAME_MAX];
	void * ptr;
} _proc;
typedef struct _proc proc;
