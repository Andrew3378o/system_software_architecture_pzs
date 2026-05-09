#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <errno.h>

static void format_permissions(mode_t mode, char *buf){
    if(S_ISREG(mode)) buf[0] = '-';
    else if(S_ISDIR(mode)) buf[0] = 'd';
    else if(S_ISLNK(mode)) buf[0] = 'l';
    else if(S_ISCHR(mode)) buf[0] = 'c';
    else if(S_ISBLK(mode)) buf[0] = 'b';
    else if(S_ISFIFO(mode)) buf[0] = 'p';
    else if(S_ISSOCK(mode)) buf[0] = 's';
    else buf[0] = '?';

    buf[1] = (mode & S_IRUSR) ? 'r' : '-';
    buf[2] = (mode & S_IWUSR) ? 'w' : '-';
    buf[3] = (mode & S_ISUID) ? 's' : (mode & S_IXUSR) ? 'x' : '-';
    buf[4] = (mode & S_IRGRP) ? 'r' : '-';
    buf[5] = (mode & S_IWGRP) ? 'w' : '-';
    buf[6] = (mode & S_ISGID) ? 's' : (mode & S_IXGRP) ? 'x' : '-';
    buf[7] = (mode & S_IROTH) ? 'r' : '-';
    buf[8] = (mode & S_IWOTH) ? 'w' : '-';
    buf[9] = (mode & S_ISVTX) ? 't' : (mode & S_IXOTH) ? 'x' : '-';
    buf[10] = '\0';
}

int main(){
    DIR *dir;
    struct dirent *entry;
    struct stat st;
    char perms[11];
    char timebuf[64];
    struct passwd *pw;
    struct group  *gr;
    time_t now;

    dir = opendir(".");
    if(dir == NULL){
        perror("opendir");
        return EXIT_FAILURE;
    }

    time(&now);

    while((entry = readdir(dir)) != NULL){
        if(strcmp(entry->d_name, ".") == 0 ||
            strcmp(entry->d_name, "..") == 0)
            continue;

        if(lstat(entry->d_name, &st) == -1){
            fprintf(stderr, "lstat(%s): %s\n", entry->d_name, strerror(errno));
            continue;
        }

        format_permissions(st.st_mode, perms);

        pw = getpwuid(st.st_uid);
        gr = getgrgid(st.st_gid);

        struct tm *tm_info = localtime(&st.st_mtime);
        if(now - st.st_mtime > 60 * 60 * 24 * 182) strftime(timebuf, sizeof(timebuf), "%b %e  %Y", tm_info);
        else strftime(timebuf, sizeof(timebuf), "%b %e %H:%M", tm_info);

        printf("%s %3lu %-8s %-8s %8lld %s %s\n",
               perms,
               (unsigned long)st.st_nlink,
               pw  ? pw->pw_name : "?",
               gr  ? gr->gr_name : "?",
               (long long)st.st_size,
               timebuf,
               entry->d_name);
    }

    closedir(dir);
    return 0;
}
