#include <stdio.h>
#include <stdlib.h>

struct sbar {
	int data[10];
};

int main(){
	struct sbar *ptr, *newptr;

	ptr = calloc(1000, sizeof(struct sbar));
	if(!ptr){
		printf("calloc failed!\n");
		return 1;
	}

	newptr = reallocarray(ptr, 500, sizeof(struct sbar));
	if(!newptr){
		printf("memory array reallocation failed!\n");
		free(ptr);
	}
	else{
		printf("array memory reallocation successed!\n");
		printf("memory reallocated at %p\n", newptr);
		free(newptr);
	}
	return 0;
}
