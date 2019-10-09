#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <errno.h>

#define DIR_SIZE 1024

void print_error(char* action) {
	printf("Error %s: %s\n", action, strerror(errno));
}

char filetype(mode_t m) {
	switch(m&S_IFMT) {
		case S_IFSOCK: return 's';
		case S_IFLNK: return 'l';
		case S_IFREG: return '-';
		case S_IFBLK: return 'b';
		case S_IFDIR: return 'd';
		case S_IFCHR: return 'c';
		case S_IFIFO: return 'p';
		default: return '?';
	}
}

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

void delete_recursive(char* filename) {
	DIR *dir;
	struct dirent *d;
	
	if ((dir = opendir(filename)) != NULL) {
		while ((d = readdir(dir)) != NULL) {

			if (!strcmp(".", d->d_name) || !strcmp("..", d->d_name)) continue;

			char* path = malloc(sizeof(filename)+sizeof(d->d_name)+sizeof(char));
			strcpy(path, filename);
			strcat(path, "/");
			strcat(path, d->d_name);

			if (d->d_type==DT_DIR) {
				if (directory_empty(path)) {
					if (rmdir(path) == -1) {
						print_error("deleting directory");
						return;					
					}
				} else {
					delete_recursive(path);
					if (rmdir(path) == -1) {
						print_error("deleting directory");	
						return;				
					}
				}
			} else {

				if (unlink(path) == -1) {
					print_error("deleting file");
					return;
				}
			}
			
			free(path);
		}	
		closedir(dir);		
	} else {
		print_error("opening directory");
	}
}

void __start(int size, char** cmd) {
	struct stat sb;

	if (size==3) {
		if (!strcmp(cmd[1], "-r")) {
			if (stat(cmd[2], &sb) == -1 ) {
				print_error("deleting file");
				return;
			}
			if (filetype(sb.st_mode)=='d') {
				char cwd[DIR_SIZE];
				getcwd(cwd, sizeof(cwd));
				delete_recursive(cmd[2]);

				if (rmdir(cmd[2]) == -1) {
					print_error("deleting parent directory");
					return;					
				}

			} else {
				printf("-r argument olny accepts directories\n");	
			}
		}
	} else if (size==2) {
		if (stat(cmd[1], &sb) == -1 ) {
			print_error("deleting file");
			return;
		}
		if (filetype(sb.st_mode)=='d') {
			if (directory_empty(cmd[1])) {
				if (rmdir(cmd[1])==-1) {
					print_error("deleting directory");
				}			
			} else {
				printf("Cannot delete non-empty folders, try delete -r instead\n");
			}
		} else {
			if (unlink(cmd[1])==-1) {
				print_error("deleting file");
			}	
		}
	}
}
