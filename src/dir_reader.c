#include <dir_reader.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

struct file_obj **listFilesInDirectory(char *dirName, int *countFiles, bool showDots) {
    DIR *d;
    struct dirent *dir;

    d = opendir(dirName);
    if (!d) {
        printf("%s %s", "Can not scan directory:", dirName);
        return NULL;
    }

    struct file_obj **files = malloc(sizeof(struct file_obj *) * 10);

    while ((dir = readdir(d)) != NULL) {
        if (dir->d_name[0] == '.' && !showDots) {
            continue;
        }

        if (*countFiles % 10 == 0) {
            files -= *countFiles;
            files = realloc(files, sizeof(struct file_obj) * (*countFiles) + sizeof(struct file_obj) * 10);
            files += *countFiles;
        }

        *countFiles++;

        struct file_obj *file = malloc(sizeof(struct file_obj));

        struct stat *st = malloc(sizeof(struct stat));
        stat(dir->d_name, st);

        file->d = dir;
        file->user = getpwuid(st->st_uid);
        file->group = getgrgid(st->st_gid);
        file->st = st;

        *files = file;
        files++;
    }

    closedir(d);

    files -= *countFiles; // reset to start

    return files;
}
