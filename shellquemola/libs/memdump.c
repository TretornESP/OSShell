#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void dump_memo(char* start, int size) {
	memmove(start, start+2, strlen(start));

	long l_start = (long)strtol(start, NULL, 16);
	char * ptr;
	ptr = (char*)l_start;

	for (int i = 0; i <= size; i++) {
		char c = *(ptr+i);

		if (i%25==0 && i != 0) {
			printf("\n");
			for (int j = i-25; j < i; j++) {
				if (*(ptr+j) <= 31) {printf(" "); continue;}
				printf("%3x", *(ptr+j));		
			}	
			printf("\n");	
		}

		if (c <= 31) {printf(" "); continue;}
		
		printf("%3c", *(ptr+i));
	} 
}

void __start(int argc, char** argv) {

	if (argc == 3) {
		dump_memo(argv[1], atoi(argv[2]));
		return;
	} else if (argc == 2) {	
		dump_memo(argv[1], 25);	
		return;
	}
	printf("Wrong syntax: memdump addr [cont]\n");
}
