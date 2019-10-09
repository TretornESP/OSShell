#ifndef SHELL_H_INCLUDED
#define SHELL_H_INCLUDED
#endif

char** cmd;
int size;

int parse(char*);
int parser();
void prompt();
int getch();
void input(char*);
int get_tokens(char*, char**);
int parse(char*);
void loop();
int main(int, char**);
