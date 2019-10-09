#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <aio.h>
#include <unistd.h>

#include "aioqueue.h"

void aio_add(struct aiocb a, int fd) {
	for (int i = 0; i < MAX; i++) {
		if (aio_array[i].valid == 0) {
			aio_pack tmp;
			tmp.cb = a;
			tmp.fd = fd;
			tmp.valid = 1;
			aio_array[i] = tmp;

			return;
		}
	}
	printf("This shell cannot keep track of so many IO operations :/ \n");
}

void aio_check() {
	for (int i = 0; i < MAX; i++) {
		// wait until the request has finished
		if(aio_array[i].valid) {
			if (aio_error(&aio_array[i].cb) != EINPROGRESS) {
				int nleidos = aio_return(&aio_array[i].cb);
				
				if (nleidos != -1)
					printf("AIO Task finished: %d B\n", nleidos);
				else
					printf("Error!");
					
				close(aio_array[i].fd);	
				aio_array[i].valid = 0;	
			} else {
				printf("At least one AIO task is in progress\n");			
			}
		}
	}
}
