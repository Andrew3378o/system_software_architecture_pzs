#include <stdio.h>
#include <stdlib.h>

void test_loop(int correct){
	void *ptr = NULL;
	int num = 10;

	while(num > 0){
		num--;

		if(!ptr){
			ptr = malloc(num);
		}

		printf("memory allocated at %p\n", ptr);

		free(ptr);

		if(correct == 1) ptr = NULL;
	}
}

int main(int argc, char *argv[]){

	if(argc < 2){
		 printf("usage: ./task4-4 [0/1]\n");
		 return 1;
	}

	test_loop(atoi(argv[1]));

	return 0;
}
