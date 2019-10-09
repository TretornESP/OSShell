#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <aio.h>

#include "../../sys/mem_list.h"

#define LEERCOMPLETO ((ssize_t)-1)

ssize_t read_async(char* fich, void *p, ssize_t n, char* p_s)
{
	typedef struct aiocb aiocb;
	ssize_t nleidos;
	ssize_t tam=n;

	int df, aux;
	struct stat s;

	if (stat (fich,&s)==-1 || (df=open(fich,O_RDONLY))==-1) return ((ssize_t)-1);
	if (n==LEERCOMPLETO) tam=(ssize_t) s.st_size;
	
	// create the control block structure
	aiocb cb;
	
	memset(&cb, 0, sizeof(aiocb));
	cb.aio_nbytes = tam;
	cb.aio_fildes = df;
	cb.aio_offset = 0;
	cb.aio_buf = p;
	
	// read!
	if (aio_read(&cb) == -1)
	{
		aux=errno;
		close(df);
		errno=aux;
		return ((ssize_t)-1);
	}
	
	printf("Request enqueued!\n");
	
	// wait until the request has finished
	while(aio_error(&cb) == EINPROGRESS)
	{
		printf("Working...\n");
	}
	
	// success?
	nleidos = aio_return(&cb);
	
	if (nleidos != -1)
		printf("Success!");
	else
		printf("Error!");
		
	// now clean up
	close(df);
	
	return nleidos;
}


ssize_t LeerFichero (char *fich, void *p, ssize_t n, char* p_s) {

	ssize_t nleidos;
	ssize_t tam=n;

	int df, aux;
	struct stat s;

	if (stat (fich,&s)==-1 || (df=open(fich,O_RDONLY))==-1) return ((ssize_t)-1);
	if (n==LEERCOMPLETO) tam=(ssize_t) s.st_size;

	//long buffer_size = get_size_by_ptr(p_s);
	//printf("THE BUFFER SIZE IS: %ld\n", buffer_size);

	if ((nleidos=read(df,p, tam))==-1){
		aux=errno;
		close(df);
		errno=aux;
		return ((ssize_t)-1);
	}
	
	close (df);
	return (nleidos);
}

void __start(int size, char** argv) {
	char* ptr;
	if (size > 2) {
		memmove(argv[2], argv[2]+2, strlen(argv[2]));

		long l_start = (long)strtol(argv[2], NULL, 16);
		ptr = (char*)l_start;
	}
	if (size == 5) {
		if (strcmp(argv[4], "-async")) {
			printf("Wrong syntax: read fich addr cont [-async]\n");
			return;
		}
		ssize_t r = read_async(argv[1], ptr, atoll(argv[3]), argv[2]);
		printf("Leidos %ld bytes de %s en %p\n", r, argv[1], ptr);
		return;	
	} else if (size == 4) {
		if (!strcmp(argv[3], "-async")) {
			ssize_t r = read_async(argv[1], ptr, -1, argv[2]);
			printf("Leidos %ld bytes de %s en %p\n", r, argv[1], ptr);
			return;			
		} else {
			ssize_t r = LeerFichero(argv[1], ptr, atoll(argv[3]), argv[2]);
			printf("Leidos %ld bytes de %s en %p\n", r, argv[1], ptr);		
			return;
		}
	} else if (size == 3) {
		ssize_t r = LeerFichero(argv[1], ptr, -1, argv[2]);
		printf("Leidos %ld bytes de %s en %p\n", r, argv[1], ptr);
		return;
	}
	printf("Wrong syntax: read fich addr cont [-async]\n");
}
