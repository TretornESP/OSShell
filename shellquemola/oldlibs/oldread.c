#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

#define LEERCOMPLETO ((ssize_t)-1)
ssize_t LeerFichero (char *fich, void *p, ssize_t n) {

	ssize_t nleidos;
	ssize_t tam=n;

	int df, aux;
	struct stat s;

	if (stat (fich,&s)==-1 || (df=open(fich,O_RDONLY))==-1) return ((ssize_t)-1);
	if (n==LEERCOMPLETO) tam=(ssize_t) s.st_size;

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

	if (size == 4) {
		ssize_t r = LeerFichero(argv[1], ptr, atoll(argv[3]));
		printf("Leidos %ld bytes de %s en %p\n", r, argv[1], ptr);		
		return;
	} else if (size == 3) {
		ssize_t r = LeerFichero(argv[1], ptr, -1);
		printf("Leidos %ld bytes de %s en %p\n", r, argv[1], ptr);
		return;
	}
	printf("Wrong syntax: read fich addr cont\n");
}
