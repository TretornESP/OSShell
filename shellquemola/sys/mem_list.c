#include <stdio.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>

#include "mem_list.h"
#include "perror.h"

void init_block_list() {
	printf("INITIALISED\n");
	MEM_LIST_LAST= 0;
	key_t key = 100;

	int shmid = shmget(key, sizeof(block_list), 0666|IPC_CREAT);
	struct mem_info* tmp = (struct mem_info*) shmat(shmid, (void*)0,0);
	memcpy(tmp, block_list, sizeof(block_list));
}

struct mem_info get_last_node() {
	return block_list[MEM_LIST_LAST];
}

void new_entry(void* ptr, long size, char* type, int fd, char* file, key_t key) {

	struct mem_info node = get_last_node();
	node.mem_ptr = ptr;
	node.mem_size = size;
   	memcpy(node.mem_type, type, strlen(type)+1);
	node.mem_fd = fd;
  	memcpy(node.mem_file, file, strlen(file)+1);
	node.mem_alloc_time = get_current_time();
	node.mem_key = key;

	block_list[MEM_LIST_LAST] = node;
	
	if (MEM_LIST_LAST > SIZE) {
		print_error("Max size exceeded");
		return;
	}

	MEM_LIST_LAST++;
}

void print_node(struct mem_info node) {
	char buffer[T_STR_BUFFER];
	time_to_string(node.mem_alloc_time, buffer, T_STR_BUFFER);

	if (!strcmp(node.mem_type, "malloc")) printf("%p: size:%ld. %s %s\n", node.mem_ptr, node.mem_size, node.mem_type, buffer);
	if (!strcmp(node.mem_type, "mmap")) printf("%p: size:%ld. %s %s (fd:%d) %s\n", node.mem_ptr, node.mem_size, node.mem_type, node.mem_file, node.mem_fd, buffer);
	if (!strcmp(node.mem_type, "shared")) printf("%p: size:%ld. %s memory (key %d) %s\n", node.mem_ptr, node.mem_size, node.mem_type, node.mem_key, buffer);

}

void print_all() {
	for (int i = 0; i < MEM_LIST_LAST; i++) {
		print_node(block_list[i]);
	}
}

int check_key_collision(key_t key) {
	for (int i = 0; i < MEM_LIST_LAST; i++) {
		if (block_list[i].mem_key == key) return 1;
	}
	return 0;
}

off_t get_key_size(key_t key) {
	for (int i = 0; i < MEM_LIST_LAST; i++) {
		if (block_list[i].mem_key == key) return block_list[i].mem_size;
	}
	return -1;
}

int find_by_size(long size, char* type) {
	for (int i = 0; i < MEM_LIST_LAST; i++) {
		if (block_list[i].mem_size == size && !strcmp(block_list[i].mem_type, type)) {
			return i;		
		}	
	}
	return -1;
}

void *dealloc_by_size(long size, char* type) {
	for (int i = 0; i < MEM_LIST_LAST; i++) {
		if (block_list[i].mem_size == size && !strcmp(block_list[i].mem_type, type)) {
			void * tmp = block_list[i].mem_ptr;			
			free(block_list[i].mem_ptr);
			remove_index(i);		
			return tmp;
		}	
	}	
	return NULL;
}

void print_type(char* type) {
	memmove(type, type+1, strlen(type));
	for (int i = 0; i < MEM_LIST_LAST; i++) {
		if (!strcmp(block_list[i].mem_type, type)) print_node(block_list[i]);
	}
}

void* get_ptr_from_index(int index) {
	return block_list[index].mem_ptr;
}

void* dealloc_first_by_file(char* file, int * fd) {
	for (int i = 0; i < MEM_LIST_LAST; i++) {
		if (!strcmp(block_list[i].mem_file, file)) {
			void* tmp = block_list[i].mem_ptr;
			if ((munmap(block_list[i].mem_ptr, block_list[i].mem_size))==-1) {
				print_error("unmaping file");
			}
			*fd = block_list[i].mem_fd;
			remove_index(i);
			return tmp;
		}
	}	
	return NULL;
}

void* dealloc_first_by_key(key_t key) {
	for (int i = 0; i < MEM_LIST_LAST; i++) {
		if (block_list[i].mem_key==key) {
			void* tmp = block_list[i].mem_ptr;
			shmdt(tmp);
			remove_index(i);
			return tmp;
		}
	}	
	return NULL;
}

long get_size_by_ptr(char* ptr) {

	for (int i = 0; i < MEM_LIST_LAST; i++) {
		char buffer[64];
		snprintf(buffer, 64, "%p", block_list[i].mem_ptr);

		if (!strcmp(buffer, ptr)) {
			return block_list[i].mem_size;
		}
		
	}
	return -1;
}

void* dealloc_by_ptr(char* ptr, char* t) {
	for (int i = 0; i < MEM_LIST_LAST; i++) {
		char buffer[64];
		snprintf(buffer, 64, "%p", block_list[i].mem_ptr);

		if (!strcmp(buffer, ptr)) {
			void* tmp = block_list[i].mem_ptr;

			if (!strcmp(block_list[i].mem_type, "malloc")) {
				free(tmp);
			} else if (!strcmp(block_list[i].mem_type, "mmap")) {
				if ((munmap(block_list[i].mem_ptr, block_list[i].mem_size))==-1) {
					print_error("unmaping file");
				}
				close(block_list[i].mem_fd);
			} else if (!strcmp(block_list[i].mem_type, "shared")) {
				shmdt(tmp);
			}

			strncpy(t, block_list[i].mem_type, strlen(block_list[i].mem_type));
			remove_index(i);
			return tmp;
		}
		
	}
	return NULL;
}

void remove_index(int index) {
	for (int i = index; i < MEM_LIST_LAST; i++) {
		block_list[i] = block_list[i + 1];	
	}
	MEM_LIST_LAST--;
}

void dump() {

}

void load() {
}


