#include <stdio.h>
#include <errno.h>
#include <string.h>

#include "perror.h"

void print_error(char* action) {
	printf("Error %s: %s\n", action, strerror(errno));
}

void print_warning(char* action) {
	printf("Warning: %s\n", action);
}
