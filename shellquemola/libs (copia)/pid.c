#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>

void __start(int size, char** cmd) {
	if (size==2) {
		if (!strcmp(cmd[1], "-p"))
			printf("Parent PID: %d\n", getppid());
	} else {
		printf("PID: %d\n", getpid());
	}
}
