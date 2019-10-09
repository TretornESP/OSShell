#include <stdio.h>

void f(int n) {
	int a[10] = {5,5,5,5,5,5,5,5,5,5};
	printf("%d\n", n);
	
	if (n==1) {
		for (int i = 0; i < 100; i++) {
			printf("[%d] %p %x\n", i, &a[i], a[i]);
		}
		return;
	}
	f(n-1);
	printf("%d\n", n);
}


int main() {
	f(7);
	printf("&f -> %p &main -> %p\n", f, main);
}

