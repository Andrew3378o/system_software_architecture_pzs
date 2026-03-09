#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/resource.h>
#include <signal.h>
#include <string.h>
#include <time.h>

#define FILENAME "result.txt"
#define LIMIT 64

void handle_signal(int signal){
	const char *str = "\nFile limit reached!\n";
	write(STDOUT_FILENO, str, strlen(str));
	exit(0);
}


int main(int argc, char *argv[]){
	if(argc < 2){
		printf("usage: %s <number of rolls>\n", argv[0]);
		return 1;
	}

	struct rlimit rl;
	int fd;
	int rolls_number = atoi(argv[1]);
	char buffer[128];

	rl.rlim_cur = LIMIT;
	rl.rlim_max = LIMIT;
	if(setrlimit(RLIMIT_FSIZE, &rl) == -1){
		perror("Error!");
		return 1;
	}
	
	signal(SIGXFSZ, handle_signal);
	fd = open(FILENAME, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if(fd == -1){
		perror("Error!");
		return 1;
	}	

	srand(time(NULL));
	
	for(int i = 1; i <= rolls_number; i++){
		int roll = (rand() % 6) + 1;
		int len = sprintf(buffer, "#%d = %d\n", i, roll);
		printf("writing to the file %s roll #%d with a value of %d\n", argv[0], i, roll);

		if(write(fd, buffer, len) == -1){
			perror("Error!");
			break;
		}
	}

	printf("Success! Exiting the programm!\n");
	close(fd);
	return 0;
}
