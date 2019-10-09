#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

void write_to_file(char* file, char* buffer, int size, int overwrite) {
	int mode = 0666;
	int flags;

	if (overwrite) {flags = O_CREAT | O_TRUNC | O_WRONLY;} else {flags = O_CREAT | O_APPEND | O_WRONLY;}

	int fd = open(file, flags, mode);
	if (fd >= 0) write(fd, buffer, size);
	close(fd);
}

void __start(int size, char** argv) {
	char* ptr;
	if (size > 3) {
		memmove(argv[2], argv[2]+2, strlen(argv[2]));

		long l_start = (long)strtol(argv[2], NULL, 16);
		ptr = (char*)l_start;
	}

	if (size == 4) {
		write_to_file(argv[1], ptr, atoi(argv[3]), 0);
		printf("Escritos %d bytes de %p en %s\n", atoi(argv[3]), ptr, argv[1]);		
		return;
	} else if (size == 5) {
		if (!strcmp(argv[4], "-o")) {
			write_to_file(argv[1], ptr, atoi(argv[3]), 1);
		} else {
			write_to_file(argv[1], ptr, atoi(argv[3]), 0);
		}
		printf("Escritos %d bytes de %s en %p\n", atoi(argv[3]), argv[1], ptr);		
		return;
	}
	printf("Wrong syntax: read fich addr cont\n");
}
