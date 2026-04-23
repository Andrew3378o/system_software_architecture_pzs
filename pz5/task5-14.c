#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(){
	long size = 10L * 1024 * 1024 * 1024; 
	
	char *ptr = malloc(size);
	if(!ptr){
		printf("memory allocation failed!\n");
		return 1;
	}

	printf("memory allocated at %p\n", (void*)ptr);
	
	long page = sysconf(_SC_PAGESIZE);
	
	for(long i = 0; i < size; i += page){
		ptr[i] = 1; 
		
		if(i % (1024L * 1024 * 1024) == 0 && i > 0){
			printf("written %lu GB\n", i / (1024 * 1024 * 1024));
		}
	}

	printf("finished successfully!\n");
	free(ptr);
	
	return 0;
}