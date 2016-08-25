
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <area51/charbuffer.h>
#include <area51/template.h>

#include "template-int.h"

Hashmap *template_hashmap = NULL;

char *template_baseDir;

extern pthread_mutex_t template_mutex;

int template_init(char *base) {
    template_baseDir = base;

    if (verbose)
        logconsole("template base %s", template_baseDir);

    pthread_mutex_init(&template_mutex, NULL);

    template_hashmap = hashmapCreate(100, hashmapStringHash, hashmapStringEquals);

    if (!template_hashmap)
        return EXIT_FAILURE;

    return EXIT_SUCCESS;

}