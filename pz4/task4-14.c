#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

void check_memory(char *msg){
	FILE *f = fopen("/proc/self/smaps_rollup", "r");
	if(!f){
		printf("failed to open smaps_rollup!\n");
		return;
	}

	char line[256];
	unsigned long priv = 0, shared = 0;

	while(fgets(line, sizeof(line), f)){
		if(strncmp(line, "Private_Dirty:", 14) == 0){
			sscanf(line + 14, "%lu", &priv);
		}
		if(strncmp(line, "Shared_Dirty:", 13) == 0){
			sscanf(line + 13, "%lu", &shared);
		}
	}
	fclose(f);

	printf("%s: private = %lu MB, shared(COW) = %lu MB\n", msg, priv / 1024, shared / 1024);
}

int main(){
	long size = 200 * 1024 * 1024;
	
	void *ptr = malloc(size);
	if(!ptr){
		printf("memory allocation failed!\n");
		return 1;
	}

	memset(ptr, 1, size);
	printf("original memory allocated at %p\n", ptr);
	check_memory("parent before fork");

	int pid = fork();
	if(pid < 0){
		printf("fork failed!\n");
		free(ptr);
		return 1;
	}

	if(pid == 0){
		printf("\nchild created successed!\n");
		check_memory("child before write");

		memset(ptr, 2, size / 2);
		printf("memory written to trigger COW!\n");

		check_memory("child after write");

		free(ptr);
		return 0;
	}

	wait(NULL);
	free(ptr);

	return 0;
}