#ifndef INC_DIR_READER_H_
#define INC_DIR_READER_H_

#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

struct file_obj {
    struct dirent *d;
    struct stat *st;
    struct passwd *user;
    struct group *group;
};

struct file_obj **listFilesInDirectory(char *dirName, int *countFiles, bool showDots);

#endif /* INC_DIR_READER_H_ */
