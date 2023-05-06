/*
 * arg_parser.c
 *
 *  Created on: 6 мая 2023 г.
 *      Author: alexandr
 */

#include <arg_parser.h>
#include <stdbool.h>
#include <defines.h>
#include <stdio.h>

int parseArguments(char **argv, bool *showDots, bool *fullInfo) {
    char *arg = argv[1];

    if (*arg != '-') {
        printf("%s %s\n", "Unknown argument:", arg);

        return EXIT_ERROR;
    }

    arg++;

    while (*arg != NULL_BYTE) {
        if (*arg == 'a') *showDots = true;
        if (*arg == 'l') *fullInfo = true;
        arg++;
    }

    return EXIT_SUCCESS;
}
