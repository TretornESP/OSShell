#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdbool.h>
#include <glob.h>
#include <dirent.h>
#include <termios.h>
#include <dlfcn.h>

//#include <openssl/md5.h>

#include "sys/shell.h"
#include "sys/banner.h"
#include "sys/perror.h"
#include "sys/memo.h"
#include "sys/libloader.h"
#include "sys/recursive2.h"

#define MAX_LINE_SIZE 128
#define DIR_SIZE 1024
#define CHUNK_SIZE 1024
#define MAX_MODULES 128

 
int parser() {
	char* tmp = malloc(sizeof(cmd[0])+sizeof(char)*4);
	strcpy(tmp, cmd[0]);
	strcat(tmp, ".so\0");

	for (int i=0; i < modules; i++) {
		if (!strcmp(tmp, lib[i].id)) {
			free(tmp);
			lib[i].function(size, cmd);
			return 0;
		}
	}

	free(tmp);
	if (is_modified()) {
		search_new_libs();
		return parser();	
	} else {
		printf("Command not found\n");
	}
	return 0;
}

void prompt() {
	char cwd[DIR_SIZE+1];
	getcwd(cwd, sizeof(cwd));
	strcat(cwd, ">");
	printf("%s", cwd);
}

int getch() {
	int ch;
	struct termios oldt;
	struct termios newt;
	tcgetattr(STDIN_FILENO, &oldt);
	newt = oldt;
	newt.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);
	ch = getchar();
	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
	return ch;
}

void input(char* string) {

	int i = 0;
		
	int index = 0;
	do {
		fflush(stdin);
		i = getch(); //USE FOR BACK AND FORTH
		if (i == 127) {
			if (index < 1) continue;
			printf("\b \b");
			index--;
			continue;
		}
		if (i == '\033') {
			getch();
			switch(getch()) {
				case 'A':
					strcpy(string, next());
					index = strlen(string);
					break;
				case 'B':
					strcpy(string, prev());
					index = strlen(string);
					break;
				case 'C':
					break;
				case 'D':
					break;
			}
		} else {
			*(string+index) = i;
			index++;
			printf("%c",i); //REMEMBER TO DECOMENT FOR BACK AND FORTH
		}
	} while (i != '\0' && i != '\n' && i != 10);
	*(string+index) = '\0';
	
	insert_after(actual_node, string); 

}

int get_tokens(char * string, char * tokens[]) { 
	int i;

	i = 1;

	if ((tokens[0]=strtok(string," \n\t"))==NULL) return 0;

	while ((tokens[i]=strtok(NULL," \n\t"))!=NULL) i++;

	return i;
}


int parse(char* string) {
	int ret;

	cmd = malloc(sizeof(char)*MAX_LINE_SIZE);
	ret = 0;
	size = get_tokens(string, cmd);
	
	if (size) {
		ret = parser();
		free(cmd);
		return ret;
	} else {
		free(cmd);	
		return 0;
	}

	return 0;
}

void loop() {
	int end;
	char* string;
	
	end = 0;
	string = malloc(sizeof(char)*MAX_LINE_SIZE);

	while (!end) {
		prompt();
		//fgets(string, sizeof(string), stdin);		
		input(string);
		end = parse(string);
	}
	
	unload_all_libs();
	free(string);
}

int main(int argc, char** argv) {
	config();
	load_all_libs();
	generate_list();
	reset_flags();
	banner();
	loop();
}
