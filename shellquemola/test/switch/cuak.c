int main();

int f() {
	 int a[10] = {5, 5, 5, 5, 5, 5, 5, 5, 5, 5};
	for (int i = 0; i < 100; i++) {
		printf("[%d] %p %x\n", i, &a[i], a[i]);	
	}
	printf("\n\n");
	int damn_digit = 0;
	damn_digit += ((a[15]/0x1000) % 0x10) * 0x1000;
	damn_digit += ((a[15]/0x100 ) % 0x10) * 0x100;
	damn_digit += ((a[15]/0x10  ) % 0x10) * 0x10;
	damn_digit += ((a[15]/0x1   ) % 0x10) * 0x1;

	printf("DAMN: %x MUST %x\n", damn_digit, (a[15]%0x10));
	if (damn_digit == (a[15]%0x10)-1) {
		printf("CORREGIR!!!\n");	
	}
	int (*ptr)() = (a[15]*0x100000000)+(a[14]*0x1);
	printf("Main at: %p\n", &main);
	printf("Need to: 0x%x%x\n", a[15], a[14]);
	printf("Jump to: %p\n\n", ptr);

	ptr();
}

int main() {
	f();
	printf("HOLA\n");
	printf("ADIOS\n");
}


