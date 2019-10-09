#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>

#include "../../sys/perror.h"
#include "../../sys/time.h"
#include "../../sys/mem_list.h"

int get_perm(char* perm) {
	int protection = 0;
	if (strlen(perm)<4) {
		if (strchr(perm,'r')!=NULL) protection|=PROT_READ;
		if (strchr(perm,'w')!=NULL) protection|=PROT_WRITE;
		if (strchr(perm,'x')!=NULL) protection|=PROT_EXEC;
	}
	return protection;
}

int open_file(char* file, int protection) {
	int mode = O_RDONLY;

	if (protection&PROT_WRITE) mode = O_RDWR;

	int f;	
	if ((f = open(file, mode)) == -1) {
		print_error("opening file");
		return -1;	
	}
	return f;
}

void close_file(int fd) {
	close(fd);
}

void call_list(void* ptr, size_t size, char* caller, int fd, char* filename, key_t key) {
	if (!ptr) {
		print_error(caller);
		return;
	}
	if (!strcmp(caller, "mmap")) {
		printf("file %s mapped at %p\n", filename, ptr);	
	} else if (!strcmp(caller, "malloc")) {
		printf("allocated %zd at %p\n", size, ptr);
	} else if (!strcmp(caller, "shared")) {
		printf ("Memoria de shmget de clave %d asignada en %p\n", key, ptr);
	}
	new_entry(ptr, size, caller, fd, filename, key);
	return;
}

size_t get_size(char* file) {
	struct stat s;
	if (stat(file, &s)==-1) {
		print_error("reading size");
		return -1;
	}
	return s.st_size;
}

void m_alloc(int size) {
	void* ptr = malloc(size);
	call_list(ptr, size, "malloc", -1, "", 0);
}

void m_dealloc(int size) {
	void* ptr = dealloc_by_size(size, "malloc");
	if (ptr == NULL) {
		printf("Couldnt deallocate, maybe wrong value?\n");
		return;	
	}
	printf("block at address %p deallocated (malloc)\n", ptr);
}

void m_map(char* file, char* permissions) {
	int protection = get_perm(permissions);
	if (!protection || protection == 2 || protection == 4 || protection == 6) {
		print_warning("So executing mmap a over a file without r permission... \ngood luck memdumping you melon");
	}
	int fd = open_file(file, protection);
	if (fd == -1) return;
	size_t size = get_size(file);
	if (size == -1) return;

	void* ptr;

	if ((ptr = mmap(NULL, size, protection, MAP_PRIVATE, fd, 0))==MAP_FAILED) {
		print_error("mapping file");
		return;
	}	

	//close_file(fd);
	call_list(ptr, size, "mmap", fd, file, 0);
}

void m_unmap(char* file) {
	int fd = -1;
	void *ptr = dealloc_first_by_file(file, &fd);
	if (ptr == NULL || fd == -1) {
		return;
	}
	close_file(fd);
	printf("block at address %p deallocated (mmap)\n", ptr);
}

void * ObtenerMemoriaShmget (key_t clave, off_t tam, int *size)
{
	void * p;
	int aux,id,flags=0777;
	struct shmid_ds s;

	if (tam) flags=flags | IPC_CREAT | IPC_EXCL;
	if (clave==IPC_PRIVATE) {errno=EINVAL; return NULL;}
	if ((id=shmget(clave, tam, flags))==-1) return (NULL);
	if ((p=shmat(id,NULL,0))==(void*) -1) {
		aux=errno;
		if (tam) shmctl(id,IPC_RMID,NULL);
		errno=aux;
		return (NULL);
	}

	shmctl (id,IPC_STAT,&s);
	*size = s.shm_segsz;
	return (p);
}

void m_shared(char* cl, char* tamc) {

	key_t key = atoi(cl);
	
	off_t tam = 0;
	int size = 0;
	void * ptr;

	if (tamc!=NULL) tam =(off_t) atoll(tamc);

	if ((ptr=ObtenerMemoriaShmget(key,tam, &size))==NULL) {
		perror ("Imposible obtener memoria shmget");
		return;
	}

	call_list(ptr, size, "shared", -1, "", key);
}

void m_unshare(char* cl) {
	key_t key = atoi(cl);
	void *ptr = dealloc_first_by_key(key);

	if (ptr == NULL) {
		return;
	}
	printf("block at address %p deallocated (shared)\n", ptr);
}

void dealloc_ptr(char* cmd) {
	char type[256];
	void *ptr = dealloc_by_ptr(cmd, type);

	if (ptr == NULL) {
		return;
	}
	printf("block at addres %p deallocated (%s)\n", ptr, type);
}

void deallocate(int size, char** cmd) {
	if (size>=2) {
		if (!strcmp(cmd[1], "-malloc")) {
			if (size==3) {
				m_dealloc(atoi(cmd[2]));			
			} else {
				print_type(cmd[1]);
			}
			return;
		} else if (!strcmp(cmd[1], "-mmap")) {
			if (size==3) {
				m_unmap(cmd[2]);
			} else {
				print_type(cmd[1]);
			}
			return;
		} else if (!strcmp(cmd[1], "-shared")) {
			if (size==3) {
				m_unshare(cmd[2]);
			} else {
				char tmp[] = "-shared";
				print_type(tmp);			
			}	
			return;	
		} else {
			if (size==2) {
				dealloc_ptr(cmd[1]);			
			}
			return;		
		}
	}
	print_all();
}


void __start(int size, char** cmd) {
	if (!strcmp(cmd[0], "deallocate")) {
		deallocate(size, cmd);
		return;	
	}
	if (size>=2) {
		if (!strcmp(cmd[1], "-malloc")) {
			if (size==3) {
				m_alloc(atoi(cmd[2]));			
			} else {
				print_type(cmd[1]);
			}
			return;
		} else if (!strcmp(cmd[1], "-mmap")) {
			if (size==3) {
				m_map(cmd[2], "");
			} else if (size==4) {
				m_map(cmd[2], cmd[3]);
			} else {
				print_type(cmd[1]);
			}
			return;
		} else if (!strcmp(cmd[1], "-createshared")) {
			if (size==4) {
				m_shared(cmd[2], cmd[3]);
			} else {
				char tmp[] = "-shared";
				print_type(tmp);			
			}	
			return;	
		} else if (!strcmp(cmd[1], "-shared")) {
			if (size==3) {
				m_shared(cmd[2], NULL);			
			} else {
				print_type(cmd[1]);
			}
			return;
			
		}
	}
	print_all();
}
