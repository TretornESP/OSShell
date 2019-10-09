#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void __start(int size, char** cmd) {
	if (size==2) {
		if (!strcmp(cmd[1], "-l")) {
			printf("xabier.iglesias.perez@udc.es\n");
			return;
		} else if (!strcmp(cmd[1], "-n")) {
			printf("Xabier Iglesias Perez\n");
			return;
		}
	} else if (size==1) {
			printf("Xabier Iglesias Perez (xabier.iglesias.perez@udc.es)\n");
			return;
	}
	printf("Wrong command syntax: autores [-l | -n]\n");
}
