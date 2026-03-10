#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/resource.h>
#include <signal.h>
#include <unistd.h>

void handle_limit(int signal){
	fprintf(stderr, "Error!\n");
	exit(EXIT_FAILURE);
}

void generate(int count, int max_val){
	int numbers[max_val];
	for(int i = 0; i < max_val; i++){
		numbers[i] = i + 1;
	}

	for(int i = 0; i < count; i++){
		int j = i + rand() % (max_val - 1);
		int temp = numbers[i];
		numbers[i] = numbers[j];
		numbers[j] = temp;
	}

	for(int i = 0; i < count; i++){
		printf("#%d = %d\n", (i + 1), numbers[i]);
	}
}

int main(){
	struct rlimit rl;
	rl.rlim_cur = 1;
	rl.rlim_max = 2;

	if(setrlimit(RLIMIT_CPU, &rl) != 0){
		perror("Error!");
		return 1;
	}

	signal(SIGXCPU, handle_limit);

	srand(time(NULL));

	printf("7 values from 1 to 49\n");
	generate(7, 49);

	printf("6 values from 1 to 36\n");
	generate(6, 36);

	//while(1);
	return 0;
}
