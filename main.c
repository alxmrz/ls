#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <defines.h>
#include <arg_parser.h>
#include <dir_reader.h>

static bool showDots = false;
static bool fullInfo = false;
static int countFiles = 0;

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
    if (argc > 1 && parseArguments(argv, &showDots, &fullInfo) == EXIT_ERROR) {
        printf("%s\n", "Failed to parse arguments");
        return EXIT_ERROR;
    }

    struct file_obj **files = listFilesInDirectory(".", &countFiles, showDots);
    if (files == NULL) {
        printf("%s\n", "Failed to scan the current directory");
        return EXIT_ERROR;
    }

    struct file_obj **original = files;

    printFiles(files);
    freeFiles(original);

    return EXIT_SUCCESS;
}
