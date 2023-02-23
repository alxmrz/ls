#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>

#define NULL_BYTE 0x00
#define EXIT_ERROR 1
#define EXIT_SUCCESS 0

struct file_obj {
    struct dirent *d;
    struct stat *st;
    struct passwd *user;
    struct group *group;
};

static bool showDots = false;
static bool fullInfo = false;
static int countFiles = 0;

static int parseArguments(char **argv) {
    char *arg = argv[1];

    if (*arg != '-') {
        printf("%s %s", "Unknown argument:", arg);

        return EXIT_ERROR;
    }

    arg++;

    while (*arg != NULL_BYTE) {
        if (*arg == 'a') showDots = true;
        if (*arg == 'l') fullInfo = true;
        arg++;
    }

    return EXIT_SUCCESS;
}

static struct file_obj **listFilesInDirectory(char *dirName) {
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

        if (countFiles % 10 == 0) {
            files -= countFiles;
            files = realloc(files, sizeof(struct file_obj) * countFiles + sizeof(struct file_obj) * 10);
            files += countFiles;
        }

        countFiles++;

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

    files -= countFiles; // reset to start

    return files;
}

static void printFiles(struct file_obj **files) {
    while (*files != NULL) {
        if (fullInfo) {
            printf(
                    "%d %s %s %5d %ld ",
                    (**files).st->st_mode,
                    (**files).user->pw_name,
                    (**files).group->gr_name,
                    (int) (**files).st->st_size,
                    (**files).st->st_mtim.tv_sec
            );

            if (DT_DIR == (**files).d->d_type) {
                printf("\033[1;34m");
            } else if ((**files).st->st_mode & S_IXUSR) {
                printf("\033[1;32m");
            }

            printf("%s", (**files).d->d_name);
            printf("\033[1;0m");

            printf("\n");
        } else {
            printf("%s ", (**files).d->d_name);
        }

        files++;
    }
    printf("\n");
}

static void freeFiles(struct file_obj **files) {
    while (*files != NULL) {
        free((*files)->st);
        free(*files);
        files++;
    }

    files -= countFiles;
    free(files);
}

int main(int argc, char **argv) {
    if (argc > 1 && parseArguments(argv) == EXIT_ERROR) {
        printf("%s\n", "Failed to parse arguments");
        return EXIT_ERROR;
    }

    struct file_obj **files = listFilesInDirectory(".");
    if (files == NULL) {
        printf("%s\n", "Failed to scan the current directory");
        return EXIT_ERROR;
    }

    struct file_obj **original = files;

    printFiles(files);
    freeFiles(original);

    return EXIT_SUCCESS;
}
