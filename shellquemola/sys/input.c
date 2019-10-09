#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>

#include "input.h"

int parser() {
	for (int i=0; i < modules; i++) {
		if (!strcmp(cmd[0], lib[i].id)) {
			lib[i].function(size, cmd);
			return 0;
		}
	}
	printf("Command not found\n");
	return 0;
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
		i = getch();
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
					next();
					break;
				case 'B':
					prev();
					break;
				case 'C':
					break;
				case 'D':
					break;
			}
		} else {
			*(string+index) = i;
			index++;
			printf("%c",i);
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
