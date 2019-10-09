#include <sys/ipc.h>
#include <sys/shm.h>

#include "time.h"
#define SIZE 4096
#define MEM_TYPE_SIZE 256
#define MEM_FILE_SIZE 8196

struct mem_info {
	void * mem_ptr;
	long mem_size;
	char mem_type[MEM_TYPE_SIZE];
	time_t mem_alloc_time;
	int mem_fd;
	char mem_file[MEM_FILE_SIZE];
	key_t mem_key;
} mem_info;

struct mem_info block_list[SIZE];
int MEM_LIST_LAST;

void init_block_list();
struct mem_info get_last_node();
void new_entry(void*r, long, char*, int, char*, key_t);

void print_node(struct mem_info);
void print_all();
void print_type(char*);
off_t get_key_size(key_t);
int check_key_collision(key_t);
int find_by_size(long, char*);
void* dealloc_by_size(long, char*);
void* get_ptr_from_index(int);
void* dealloc_first_by_file(char*, int*);
void* dealloc_first_by_key(key_t);
long get_size_by_ptr(char*);
void* dealloc_by_ptr(char*, char*);
void remove_index(int);
void dump();
void load();
