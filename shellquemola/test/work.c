#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <elf.h>

#include "symbol_table.h"

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
	    return 1;
	
	fseek(infile, 0L, SEEK_END);
	numbytes = ftell(infile);
	 
	fseek(infile, 0L, SEEK_SET);	
	 
	buffer = (unsigned char*)calloc(numbytes, sizeof(unsigned char));	
	 
	if(buffer == NULL)
	    return 1;
	 
	fread(buffer, sizeof(unsigned char), numbytes, infile);
	fclose(infile);

	struct symbol* hd = get_symbol(file_name, "__start");

	if (hd) {

		printf("NUMBYTES RW: %ld\n", numbytes);
		printf("ENTRY -> %x\n", hd->addr);

		memcpy(m, buffer, numbytes);
		return hd->addr;
	} else {
		printf("error");
	}
	free(buffer);
	return 0;
}

void memdump(char* ptr, int size) {
	for (int i = 0; i < size; i++)	{
		//if (i%8==0) printf(" ");
		printf("%x", *(ptr+i));
	}
}

const size_t SIZE = 8196;
typedef void (*JittedFunc)(int, char**);

void __start(int argc, char** argv) {
	  void* m = alloc_executable_memory(SIZE);
	  int offset = emit_code_into_memory(argv[1], m);
		
	  printf("M before: %p\n", m);
	  m+=offset;
	  memdump(m, 100);
	
	  JittedFunc func = m;
	  printf("\n\nPTR -> %p\n", func);

	  func(argc, argv);
}
