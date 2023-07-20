#include <stdio.h>

int main() {
	int fat = 1;
	int n = 8;

	while( n > 0 ) {
		
		fat = fat * n;
		n = n - 1;
	}
	printf("%d\n", fat);
	return 0;
}