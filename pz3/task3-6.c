#include <stdio.h>
#include <stdlib.h>

void func(int curr, int max) {
	if(curr >= max) return;

    	char buffer[16384];

    	printf("current recursion depth: %d (%d B used)\n", curr, curr * 16384);

    	buffer[0] = (char)(curr % 256);
	func(curr + 1, max);
}

int main(int argc, char *argv[]) {
    	if (argc < 2) {
        	printf("usage: %s <depth of recursion>\n", argv[0]);
        	return 1;
	}

    	int d = atoi(argv[1]);

    	if (d <= 0) {
        	printf("Test completed!\n");
        	return 0;
    	}

    	printf("Testing stack:\n");
    	func(1, d);

    	printf("Test completed!\n");
    	return 0;
}
