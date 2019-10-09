#ifndef RECURSIVE_H_INCLUDED
#define RECURSIVE_H_INCLUDED
#endif

int directory_empty(char*);

void recursive(void (*func)(const char*), const char*);
