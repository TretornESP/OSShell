#include "time.h"
#include <stdio.h>

time_t get_current_time() {
	time_t t;
	return time(&t);
}


void time_to_string(time_t t, char* buffer, int size) {
	struct tm* info;
	
	info = localtime(&t);

	strftime(buffer, size,"%a %b %d %H:%M:%S %Y", info);

	return;
}

void print_current_time() {
	time_t t = get_current_time();

	char buffer[T_STR_BUFFER];
	time_to_string(t, buffer, T_STR_BUFFER);
	printf("%s\n", buffer);
}

void current_time_to_string(char* buffer) {
	time_t t = get_current_time();
	time_to_string(t, buffer, T_STR_BUFFER);

}
