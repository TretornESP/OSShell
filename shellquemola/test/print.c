#include <stdio.h>
#include <stdlib.h>


int main(int argc, char** argv) {
	FILE    *infile;
	unsigned char    *buffer;
	long    numbytes;
	 
	infile = fopen(argv[1], "r");
	 
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

	for (int i = 0; i < numbytes; i++) {
		printf("%x ", buffer[i]);
	}
	 	 
	free(buffer);
}
