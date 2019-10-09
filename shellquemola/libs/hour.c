#include <time.h>
#include <stdio.h>

void __start(int size, char** argc) {
	time_t rawtime;
	struct tm * timeinfo;

  	time (&rawtime);
  	timeinfo = localtime(&rawtime);
  	printf ("%s", asctime(timeinfo));
}

