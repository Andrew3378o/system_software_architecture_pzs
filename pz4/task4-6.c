#include <stdio.h>
#include <stdlib.h>

int main(){

	void *ptr = malloc(1024);
	if(!ptr){
		printf("memory allocation failed!\n");
		return 1;
	}

	printf("original memory allocated at %p\n", ptr);

	printf("test #1: realloc(NULL, 1024)\n");

	void *test1 = realloc(NULL, 1024);
	if(!test1){
		printf("memory reallocation at the test #1 failed!\n");
	}
	else{
		printf("memory reallocation at the test #1 successed!\n");
		printf("memory reallocated at %p\n", test1);
		free(test1);
	}

	printf("test #2: realloc(ptr, 0)\n");

	void *test2 = realloc(ptr, 0);
	if(!test2){
		printf("memory reallocation at the test #2 freed memory of ptr!\n");
	}
	else{
		printf("memory reallocation at the test #2 successed!\n");
		printf("memory reallocated at %p\n", test2);
		free(test2);
	}
	return 0;
}
