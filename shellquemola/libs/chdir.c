#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define DIR_SIZE 1024

void __start(int size, char** cmd) {	
	if (size==2) {
		if (chdir(cmd[1])) printf("Cannot access the specified directory\n");	
	} else {
		char cwd[DIR_SIZE];
		if (getcwd(cwd, sizeof(cwd)) != NULL) {
			printf("%s\n", cwd);
		} else {
			printf("Cannot resolve directory name\n");
		}
	}
}
