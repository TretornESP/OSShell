#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define MAX_LINE 8196

void __start(int argc, char **argv) {
	char* cmd = malloc(sizeof(char)*MAX_LINE);

	if (argc<2) {
		printf("Wrong syntax: system cmd params params...\n");
		return;
	}

	strcpy(cmd, argv[1]);

	for (int i = 2; i < argc; i++) {
		sprintf(cmd, "%s %s", cmd, argv[i]);
	}

	system(cmd);
	free(cmd);
}
