#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

void print_error(char* action) {
	printf("Error %s: %s\n", action, strerror(errno));
}

void __start(int size, char** cmd) {
	if (size==3) {
		if (!strcmp(cmd[1], "-d")) {
			if (!mkdir(cmd[2],  S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH)) return;
		}
	} else if (size==2) {
		if (creat(cmd[1], S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH)>=0) return;
	}
	print_error("creating file");
}

