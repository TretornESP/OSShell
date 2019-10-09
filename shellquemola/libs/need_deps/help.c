#include <stdio.h>
#include "../../sys/libloader.h"

void __start(int argc, char** argv) {
	printf("%d COMMANDS\n", modules);
	for (int i = 0; i < modules; i++) {
		printf("%s\n", lib[i].id);
	}
}
