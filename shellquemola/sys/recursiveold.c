#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>

#include "recursive.h"
#include "perror.h"

int directory_empty(char* filename) {
	int n = 0;

	struct dirent *d;
	
	DIR *dir = opendir(filename);
	if (dir == NULL) {
		printf("Directory doesn't exist\n");
		return 0;
	}
	
	while((d = readdir(dir)) != NULL) {
		if (++n > 2) {
			closedir(dir);
			return 0;		
		}
	}

	closedir(dir);
	return 1;
}

void recursive(void (*func)(const char*), const char *basePath) {
    char path[4096];
    struct dirent *dp;
    DIR *dir = opendir(basePath);

    // Unable to open directory stream
    if (!dir) return;

    while ((dp = readdir(dir)) != NULL) {
        if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0) {


            // Construct new path from our base path
            strcpy(path, basePath);
            strcat(path, "/");
            strcat(path, dp->d_name);
	    strcat(path, "\0");

	    (*func)(path);

            recursive(func, path);
        }
    }

    closedir(dir);
}

/*
void recursive(void (*func)(const char*), char* filename) {
	DIR *dir;
	struct dirent *d;
		
	if ((dir = opendir(filename)) != NULL) {
		while ((d = readdir(dir)) != NULL) {

			if (!strcmp(".", d->d_name) || !strcmp("..", d->d_name)) continue;

			char* path = malloc(sizeof(filename)+sizeof(d->d_name)+sizeof(char)*2);
			strcpy(path, filename);
			strcat(path, "/");
			strcat(path, d->d_name);
			strcat(path, "\0");

			if (d->d_type==DT_DIR) {
				if (directory_empty(path)) {
					(*func)(path);
				} else {
					recursive(func, path);
					(*func)(path);
				}
			} else {
				(*func)(path);
			}
			free(path);
		}
		closedir(dir);
	} else {
		print_error("opening directory");
	}
} */
