#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <string.h>

void check_disk_state(const char *filename, const char *stage) {
    char buf[32] = {0};
    int fd = open(filename, O_RDONLY);
    read(fd, buf, sizeof(buf) - 1);
    close(fd);
    printf("[%s] disk content:   '%s'\n", stage, buf);
}

void simulate_memory_model(int flags, const char *mode_name, const char *new_text) {
    printf("\nsimulation: %s\n", mode_name);
    
    const char *filename = "simulation.txt";
    int fd = open(filename, O_RDWR | O_CREAT | O_TRUNC, 0644);
    write(fd, "INITIAL_DATA", 12);
    
    char *ptr = mmap(NULL, 32, PROT_READ | PROT_WRITE, flags, fd, 0);
    
    check_disk_state(filename, "before write");

    strcpy(ptr, new_text);
    printf("[memory state] pointer value: '%s'\n", ptr);
    
    printf("    calling msync()...\n");
    msync(ptr, 32, MS_SYNC);
    
    check_disk_state(filename, "after msync ");

    munmap(ptr, 32);
    close(fd);
}

int main() {
    simulate_memory_model(MAP_PRIVATE, "MAP_PRIVATE (Copy-on-Write)", "PRIVATE_DATA");
    simulate_memory_model(MAP_SHARED,  "MAP_SHARED (Direct Cache)",   "SHARED_DATA_");
    return 0;
}