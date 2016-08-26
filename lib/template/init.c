
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <area51/charbuffer.h>
#include <area51/template.h>

#include "template-int.h"

TemplateEngine *template_init(char *base) {
    TemplateEngine *e = malloc(sizeof (struct TemplateEngine));
    if (!e)
        return NULL;

    memset(e, 0, sizeof (struct TemplateEngine));
    e->baseDir = base;

    pthread_mutex_init(&e->mutex, NULL);

    e->templates = hashmapCreate(100, hashmapStringHash, hashmapStringEquals);

    if (!e->templates) {
        free(e);
        return NULL;
    }

    return e;

}