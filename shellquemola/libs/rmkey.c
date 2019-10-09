#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>

void __start(int argc, char **argv) {

	if (argc != 2) {
		printf("Wring syntax: rmkey [cl]\n");
		return;
	}

	char* keyc = argv[1]; 
	key_t clave;
	int id;

	if (keyc==NULL || (clave=(key_t) strtoul(keyc,NULL,10))==IPC_PRIVATE) {
		printf ("   rmkey  clave_valida\n");
		return;
	}

	if ((id=shmget(clave,0,0666))==-1) {
		perror ("shmget: imposible obtener memoria compartida");
		return;
	}

	if (shmctl(id,IPC_RMID,NULL)==-1) {
		perror ("shmctl: imposible eliminar memoria compartida\n");
	}
}
