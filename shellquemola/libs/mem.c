#include <stdio.h>

void dummy1() {
}

void dummy2() {
}

void dummy3() {
}

int extern1;
int extern2;
int extern3;

void __start(int argc, char** argv) {
	auto int auto1;
	auto int auto2;
	auto int auto3;

	printf("Functions: %p %p %p\n", &dummy1, &dummy2, &dummy3);
	printf("Extern: %p %p %p\n", &extern1, &extern2, &extern3);
	printf("Auto: %p %p %p\n", &auto1, &auto2, &auto3);
}
