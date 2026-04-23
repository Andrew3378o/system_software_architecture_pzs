#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int main(){

	void *ptr = malloc(1024);
	if(!ptr){
		printf("memory allocation failed!\n");
		return 1;
	}

	printf("original memory allocated at %p\n", ptr);

	void *tmp = realloc(ptr, SIZE_MAX);
	if(!tmp){
		printf("memory reallocation failed!\n");
		printf("original pointer at %p is still valid.\n", ptr);

		free(ptr);
		printf("original memory safely freed.\n");
	}
	else{
		printf("reallocation successful at %p\n", tmp);
		free(tmp);
	}

	return 0;
}
