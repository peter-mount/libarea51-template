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

extern Hashmap *template_hashmap;

void template_free_internal(TemplateEngine *e, TemplateFile *f) {
    if (!f)
        return;

    if (f->key) {
        hashmapRemove(e->templates, f->key);
        if (f->freeKey)
            f->freeKey(f->key);
    }

    if (f->buffer) {
        if (f->memmapped)
            munmap(f->buffer, f->size);
        else if (f->freePos)
            f->freePos(f->buffer);
    }

    if (f->fd >= 0)
        close(f->fd);

    free(f);
}

void template_free(TemplateEngine *e, TemplateFile *f) {
    if (!f)
        return;

    template_lock(e);

    f->useCount--;

    if (!f->permanent && f->useCount < 1) {

        if (verbose)
            logconsole("Free template %s", f->key);

        template_free_internal(e, f);
    }

    template_unlock(e);
}
