#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <elf.h>

#include "../../sys/symbol_table.h"

// Allocates RWX memory of given size and returns a pointer to it. On failure,
// prints out the error and returns NULL.
void* alloc_executable_memory(size_t size) {
  void* ptr = mmap(0, size,
                   PROT_READ | PROT_WRITE | PROT_EXEC,
                   MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
  if (ptr == (void*)-1) {
    perror("mmap");
    return NULL;
  }
  return ptr;
}

int emit_code_into_memory(const char* file_name, unsigned char* m) {
	FILE    *infile;
	unsigned char    *buffer;
	long    numbytes;
	 
	infile = fopen(file_name, "r");
	 
	if(infile == NULL)
	    return -1;
	
	fseek(infile, 0L, SEEK_END);
	numbytes = ftell(infile);
	 
	fseek(infile, 0L, SEEK_SET);	
	 
	buffer = (unsigned char*)calloc(numbytes, sizeof(unsigned char));	
	 
	if(buffer == NULL)
	    return -1;
	 
	fread(buffer, sizeof(unsigned char), numbytes, infile);
	fclose(infile);

	struct symbol* hd = get_symbol(file_name, "__start");

	if (hd) {

		printf("[LOADER] NUMBYTES RW: %ld\n", numbytes);
		printf("[LOADER] ENTRY -> %x\n", hd->addr);

		memcpy(m, buffer, numbytes);
		return hd->addr;
	} else {
		printf("[LOADER] error");
		free(buffer);
		return -1;
	}
	free(buffer);
	return 0;
}

void memdump(char* ptr, int size) {
	printf("[LOADER] DUMPING %d FIRST BYTES OF REGION\n", size);
	for (int i = 0; i < size; i++)	{
		//if (i%8==0) printf(" ");
		printf("%x", *(ptr+i));
	}
	printf("\n[LOADER] End of dump\n");
}

const size_t SIZE = 8196;
typedef int (*JittedFunc)(int, char**);

void __start(int argc, char** argv) {
	  void* m = alloc_executable_memory(SIZE);
	  int offset = emit_code_into_memory(argv[1], m);

	  if (offset<0) {printf("[LOADER] ERROR LOADING FILE\n"); return;}
		
	  printf("[LOADER] M before: %p\n", m);
	  m+=offset;
	  memdump(m, 100);
	
	  JittedFunc func = m;
	  printf("\n[LOADER] PTR -> %p\n", func);
	  printf("\n\n**********************[EXECUTING REGION]**********************\n");
	  int res = func(argc, argv);

	  printf("RESULT: %d\n", res);
}
