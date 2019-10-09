#include <time.h>
#include <stdio.h>

void __start(int size, char** cmd) {
	time_t t;
	struct tm tm;

	t =time(NULL);
	tm = *localtime(&t);

	printf("%d-%d-%d\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);
}
