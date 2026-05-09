#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>

#define COPY_BUF_SIZE 65536
#define PATH_SZ       4096
#define MAX_FOUND     1024

typedef struct {
    pid_t pid;
    int   fd;
    char  orig_path[PATH_SZ];
    char  fd_path[PATH_SZ];
    off_t size;
} DeletedFile;

static DeletedFile found[MAX_FOUND];
static int found_count = 0;

static void format_size(off_t size, char *buf, size_t bufsz){
    if(size < 1024) snprintf(buf, bufsz, "%lld B", (long long)size);
    else if(size < 1024 * 1024) snprintf(buf, bufsz, "%.1f KB", (double)size / 1024.0);
    else snprintf(buf, bufsz, "%.1f MB", (double)size / (1024.0 * 1024.0));
}

static void scan_process(pid_t pid){
    char fd_dir[PATH_SZ];
    DIR *dir;
    struct  dirent *ent;
    char link_path[PATH_SZ];
    char link_target[PATH_SZ];
    ssize_t len;
    struct stat st;

    snprintf(fd_dir, sizeof(fd_dir), "/proc/%d/fd", (int)pid);

    dir = opendir(fd_dir);
    if(dir == NULL) return;

    while((ent = readdir(dir)) != NULL){
        if(ent->d_name[0] == '.') continue;

        snprintf(link_path, sizeof(link_path), "/proc/%d/fd/%s", (int)pid, ent->d_name);

        len = readlink(link_path, link_target, sizeof(link_target) - 1);
        if(len == -1) continue;
        link_target[len] = '\0';

        if(strstr(link_target, " (deleted)") == NULL) continue;

        if(found_count >= MAX_FOUND) break;

        found[found_count].pid = pid;
        found[found_count].fd  = atoi(ent->d_name);
        strncpy(found[found_count].fd_path, link_path, PATH_SZ - 1);

        strncpy(found[found_count].orig_path, link_target, PATH_SZ - 1);
        char *suffix = strstr(found[found_count].orig_path, " (deleted)");
        if(suffix) *suffix = '\0';

        if(stat(link_path, &st) == 0) found[found_count].size = st.st_size;
        else found[found_count].size = -1;

        found_count++;
    }
    closedir(dir);
}

static void scan_all_processes(){
    DIR *proc_dir;
    struct dirent *ent;
    pid_t pid;
    char *endptr;

    proc_dir = opendir("/proc");
    if(proc_dir == NULL){
        perror("opendir /proc");
        exit(1);
    }

    while((ent = readdir(proc_dir)) != NULL){
        pid = (pid_t)strtol(ent->d_name, &endptr, 10);
        if(*endptr != '\0' || pid <= 0) continue;
        scan_process(pid);
    }

    closedir(proc_dir);
}

static int copy_file(const char *src_path, const char *dst_path){
    int src_fd, dst_fd;
    char buf[COPY_BUF_SIZE];
    ssize_t nread, nwritten;
    int ret = 0;

    src_fd = open(src_path, O_RDONLY);
    if(src_fd == -1){
        fprintf(stderr, "Не вдалося відкрити джерело '%s': %s\n", src_path, strerror(errno));
        return -1;
    }

    dst_fd = open(dst_path, O_WRONLY | O_CREAT | O_EXCL, 0644);
    if(dst_fd == -1){
        if(errno == EEXIST) fprintf(stderr, "Файл '%s' вже існує. Пропускаємо.\n", dst_path);
        else fprintf(stderr, "Не вдалося створити '%s': %s\n", dst_path, strerror(errno));
        close(src_fd);
        return -1;
    }

    while((nread = read(src_fd, buf, sizeof(buf))) > 0){
        char *ptr = buf;
        ssize_t remaining = nread;

        while(remaining > 0){
            nwritten = write(dst_fd, ptr, (size_t)remaining);
            if(nwritten == -1){
                fprintf(stderr, "Помилка запису у '%s': %s\n", dst_path, strerror(errno));
                ret = -1;
                goto done;
            }
            ptr += nwritten;
            remaining -= nwritten;
        }
    }

    if(nread == -1){
        fprintf(stderr, "Помилка читання з '%s': %s\n", src_path, strerror(errno));
        ret = -1;
    }

done:
    close(src_fd);
    close(dst_fd);

    if(ret == -1) unlink(dst_path);

    return ret;
}

static void list_found(void){
    int i;
    char szstr[32];

    if(found_count == 0){
        printf("Вилучених файлів не знайдено.\n");
        printf("(Файл можна відновити лише якщо хоча б один процес\nтримає його відкритим на момент сканування)\n");
        return;
    }

    printf("\nЗнайдено вилучених файлів: %d\n", found_count);
    printf("%-6s %-5s %-10s %s\n", "PID", "FD", "Розмір", "Оригінальний шлях");
    printf("%-6s %-5s %-10s %s\n", "------", "-----", "----------", "--------------------------------------------");

    for(i  = 0; i < found_count; i++){
        if(found[i].size >= 0) format_size(found[i].size, szstr, sizeof(szstr));
        else snprintf(szstr, sizeof(szstr), "?");
        printf("%-6d %-5d %-10s %s\n", (int)found[i].pid, found[i].fd, szstr, found[i].orig_path);
    }
}

static DeletedFile *find_by_path(const char *orig_path){
    int i;
    for(i = 0; i < found_count; i++){
        if(strcmp(found[i].orig_path, orig_path) == 0) return &found[i];
    }
    return NULL;
}

static int recover_one(const char *orig_path, const char *dest_path){
    DeletedFile *df = find_by_path(orig_path);
    if(df == NULL){
        fprintf(stderr, "Файл '%s' не знайдено серед вилучених.\n", orig_path);
        return -1;
    }

    printf("Відновлення: %s\n  джерело : %s\n  збереження: %s\n", df->orig_path, df->fd_path, dest_path);

    if(copy_file(df->fd_path, dest_path) == 0){
        printf("  OK\n");
        return 0;
    }
    return -1;
}

static void recover_all(const char *dest_dir){
    int i;
    char dest_path[PATH_SZ * 2];
    char base[PATH_SZ];
    char *slash;
    int ok = 0, fail = 0;

    for(i = 0; i < found_count; i++){
        strncpy(base, found[i].orig_path, PATH_SZ - 1);
        slash = strrchr(base, '/');
        snprintf(dest_path, sizeof(dest_path) - 1, "%s/%s", dest_dir, slash ? slash + 1 : base);

        printf("[%d/%d] %s -> %s\n", i + 1, found_count, found[i].orig_path, dest_path);

        if(copy_file(found[i].fd_path, dest_path) == 0) ok++;
        else fail++;
    }

    printf("\nГотово: відновлено %d, помилок %d.\n", ok, fail);
}

static void print_usage(const char *prog){
    fprintf(stderr,
        "Використання:\n"
        "  %s                          - показати всі вилучені відкриті файли\n"
        "  %s -r <orig_path> <dest>    - відновити один файл\n"
        "  %s -a <dest_dir>            - відновити всі файли у каталог\n"
        "\nПриклад:\n"
        "  %s\n"
        "  %s -r /tmp/important.txt ./recovered.txt\n"
        "  %s -a ./recovered/\n",
        prog, prog, prog, prog, prog, prog);
}

int main(int argc, char *argv[]){
    printf("Сканування /proc ...\n");
    scan_all_processes();

    if(argc == 1){
        list_found();
        return 0;
    }

    if(argc == 4 && strcmp(argv[1], "-r") == 0){
        list_found();
        return recover_one(argv[2], argv[3]) == 0 ? 0 : 1;
    }

    if(argc == 3 && strcmp(argv[1], "-a") == 0){
        list_found();
        if(found_count > 0) recover_all(argv[2]);
        return 0;
    }

    print_usage(argv[0]);
    return 1;
}