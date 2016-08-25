#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <area51/charbuffer.h>
#include <area51/hashmap.h>
#include <area51/log.h>
#include <area51/string.h>
#include <area51/comparator.h>
#include <area51/template.h>

#include <fcntl.h>
#include <elf.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "template-int.h"

int template_load_internal(TemplateEngine *e, char *name) {
    if (verbose)
        logconsole("Load template %s", name);

    TemplateFile *f = malloc(sizeof (struct TemplateFile));
    if (!f)
        return EXIT_FAILURE;

    memset(f, 0, sizeof (struct TemplateFile));
    f->fd = -1;

    f->key = strdup(name);
    if (!f->key) {
        template_free_internal(e, f);
        return EXIT_FAILURE;
    }

    char *n = genurl(template_baseDir, name);
    if (!n) {
        template_free_internal(e, f);
        return EXIT_FAILURE;
    }

    f->fd = open(n, O_RDONLY);
    free(n);

    if (f->fd == -1) {
        template_free_internal(e, f);
        return EXIT_FAILURE;
    }

    struct stat sb;
    if (fstat(f->fd, &sb) == -1) {
        template_free_internal(e, f);
        return EXIT_FAILURE;
    }

    f->size = sb.st_size;
    f->buffer = mmap(NULL, f->size, PROT_READ, MAP_PRIVATE, f->fd, 0);
    if (f->buffer == MAP_FAILED) {
        template_free_internal(e, f);
        return EXIT_FAILURE;
    }

    hashmapPut(e->templates, name, f);
    return EXIT_SUCCESS;
}
