#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 1024

int main(int argc, char *argv[]){
	if(argc != 3){
		fprintf(stderr, "Usage: %s <filename> <number of lines>\n", argv[0]);
		return 1;
	}

	FILE *fp = fopen(argv[1], "r");
	if(!fp){
		perror("File opening failed!\n");
		return 1;
	}

	int n = atoi(argv[2]);
	if(n < 0){
		perror("An invalid number!\n");
	}

	int capacity = 10;
	int count = 0;
	char **lines = malloc(capacity * sizeof(char*));

	char buffer[MAX];

	while(fgets(buffer, MAX, fp)){
		if(count >= capacity){
			capacity *= 2;
			lines = realloc(lines, capacity * sizeof(char*));
			if(!lines){
				fprintf(stderr, "Memory allocation failed!\n");
				return 1;
			}
		}
		lines[count] = malloc(strlen(buffer) + 1);
		strcpy(lines[count], buffer);
		count++;
	}

	for(int i = count - 1; i >= count - n; i--){
		printf("%s", lines[i]);
		free(lines[i]);
	}

	free(lines);
	fclose(fp);
	return 0;
}
