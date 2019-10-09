#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <aio.h>

void write_async(char* file, char* buffer, int size, int overwrite) {
	typedef struct aiocb aiocb;
	int mode = 0666;
	int flags;
	int aux;
	ssize_t nescritos;

	aiocb cb;

	if (overwrite) {flags = O_CREAT | O_TRUNC | O_WRONLY;} else {flags = O_CREAT | O_APPEND | O_WRONLY;}


	int fd = open(file, flags, mode);

	if (fd >= 0) {
		memset(&cb, 0, sizeof(aiocb));
		cb.aio_nbytes = size;
		cb.aio_fildes = fd;
		cb.aio_offset = 0;
		cb.aio_buf = buffer;

		if (aio_write(&cb) == -1) {
			aux=errno;
			close(fd);
			errno=aux;
		}

		printf("Request enqueued!\n");

		while(aio_error(&cb) == EINPROGRESS) {
			printf("Working...\n");
		}
		
		nescritos = aio_return(&cb);
	
		if (nescritos != -1)
			printf("Success!");
		else
			printf("Error!");
			
		// now clean up
		close(fd);	
	} else {
		printf("Couldnt open file\n");	
	}
}

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
	} else if (size >= 5) {
		int o = 0;
		int a = 0;
		for (int i = 3; i < size; i++) {
			if (!strcmp(argv[i], "-o")) o = 1;
			if (!strcmp(argv[i], "-async")) a = 1;		
		}
		if ( o && a) {
			write_async(argv[1], ptr, atoi(argv[3]), 1);
			printf("Escritos %d bytes de %s en %p\n", atoi(argv[3]), argv[1], ptr);		
			return;		
		}

		if (o) {
			write_to_file(argv[1], ptr, atoi(argv[3]), 1);
		}

		if (a) {
			write_async(argv[1], ptr, atoi(argv[3]), 0);
		}

		printf("Escritos %d bytes de %s en %p\n", atoi(argv[3]), argv[1], ptr);		
		return;
	}
	printf("Wrong syntax: read fich addr cont\n");
}
