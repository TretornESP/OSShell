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
#include <unistd.h>

#include "../../sys/perror.h"
#include "../../sys/time.h"
#include "../../sys/mem_list.h"
#include "../../sys/libloader.h"

void __start(int size, char** cmd) {
  	char cwd[256];
	getcwd(cwd, sizeof(cwd));
	load_lib("./libs/need_deps/allocate.so");
	lib[0].function(size, cmd);
}
