#ifndef INPUT_H_INCLUDED
#define INPUT_H_INCLUDED
#endif

char** cmd;
int size;

int parser();
int getch();
void input(char*);
int get_tokens(char * string, char **); 
int parse(char*);
