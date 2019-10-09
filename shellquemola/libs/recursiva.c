#include <stdio.h>
#include <stdlib.h>

#define TAMANO 2048
#define DEATH 3995

void recursiva (int n) {
	char automatico[TAMANO];
	static char estatico[TAMANO];
	printf ("parametro n:%d en %p\n",n,&n);
	printf ("array estatico en:%p \n",estatico);
	printf ("array automatico en %p\n",automatico);

	n--;
	if (n>0) recursiva(n);
}

void __start(int argc, char** argv) {
	int a = atoi(argv[1]);
	if (a > DEATH) {
		printf("Warning: given number is bigger than %d which means segfault.\n", DEATH);
		printf("(if you are a coward type an 'n' to return or else press intro and get ready to meet your maker)\n");
		char save_me = getchar();
		if (save_me == 'n') return;
	}
	recursiva(a);
	return;
	printf("Wrong syntax: recursiva [n]\n");
}
