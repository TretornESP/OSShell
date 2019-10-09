#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <glob.h>
#include <pwd.h>
#include <grp.h>
#include <unistd.h> 
#include <time.h>
#include <errno.h>
#include <dirent.h>

glob_t wildcard(const char* input) {
	glob_t glob_result;
	memset(&glob_result, 0, sizeof(glob_result));
	int return_value = glob(input, GLOB_TILDE, NULL, &glob_result);
   	if(return_value != 0) {
		printf("File %s not found\n", input);
        	globfree(&glob_result);
    	}
	return glob_result;
}

void print_error(char* action) {
	printf("Error %s: %s\n", action, strerror(errno));
}

int isDirectory(const char *path) {
  	struct stat statbuf;
 	if (stat(path, &statbuf) != 0)	return 0;
  	return S_ISDIR(statbuf.st_mode);
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

void convierte_modo(mode_t m, char* permisos) {
	strcpy(permisos, "---------- ");

	permisos[0]=filetype(m);
	if (m&S_IRUSR) permisos[1]='r'; /*propietario*/
	if (m&S_IWUSR) permisos[2]='w';
	if (m&S_IXUSR) permisos[3]='x';
	if (m&S_IRGRP) permisos[4]='r';   /*grupo*/
	if (m&S_IWGRP) permisos[5]='w';
	if (m&S_IXGRP) permisos[6]='x';
	if (m&S_IROTH) permisos[7]='r';   /*resto*/
	if (m&S_IWOTH) permisos[8]='w';
	if (m&S_IXOTH) permisos[9]='x';
	if (m&S_ISUID) permisos[3]='s';  /*setuid, setgid y stickybit*/
	if (m&S_ISGID) permisos[6]='s';
	if (m&S_ISVTX) permisos[9]='t';
}

void query_file(const char* path) {
	struct stat sb;
	struct passwd *usr_pwd;
	struct group *grp_pwd;

	char* permisos = malloc (12* sizeof(char));

	if (lstat(path, &sb) == -1) {
		print_error("querying file");
		return;			
	}		
	convierte_modo(sb.st_mode, permisos);
	usr_pwd = getpwuid(sb.st_uid);
	grp_pwd = getgrgid(sb.st_gid);
	char date[128];
	strftime(date, 36, "%b %d %H:%M", localtime(&sb.st_mtime));

	int index = 0;
	for (int i = 0; i < (strlen(path)); i++) {
		if (path[i] == '/') {
			index = i+1;
		}
	}

	char path_new[strlen(path)];

	for (int i = index; i < strlen(path); i++) {
		*(path_new+(i-index)) = *(path+i);
	}

	char * filename;
	if (filetype(sb.st_mode)=='l') {
		char buf[512];
		filename = malloc (sizeof(path_new) + sizeof(char)*6 + sizeof(buf));
		if (readlink(path, buf, sizeof(buf)) == -1) {
			print_error("reading link");
			return;				
		}
		strcpy(filename, path_new);
		strcat(filename, " -> ");
		strcat(filename, buf);
		strcat(filename, "\0");
	} else {
		filename = malloc (sizeof(path_new)+sizeof(char));
		strcpy(filename, path_new);		
		strcat(filename, "\0");	
	}

	printf("%lu %s %lu %s %s %ld %s %s\n", sb.st_ino, permisos, sb.st_nlink, usr_pwd->pw_name, grp_pwd->gr_name, sb.st_size ,date, filename);

	free(filename);	
	free(permisos);
}

void __start(int size, char** cmd) {
	if (size>1) {
		for (int i = 1; i < size ; i++) {
			glob_t result = wildcard(cmd[i]);
			for (int j = 0; j < result.gl_pathc; j++) {
				query_file(result.gl_pathv[j]);
		 	}
		}			
	} else {
		printf("Usage: query <file1> <file2> <..>\n");	
	}
}
