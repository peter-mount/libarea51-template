#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <area51/hashmap.h>
#include <area51/template.h>

#include <fcntl.h>
#include <elf.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "template-int.h"

void template_free_internal(TemplateFile *f) {
    if (!f)
        return;

    if (f->key) {

        if (f->owner)
            hashmapRemove(f->owner->templates, f->key);

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

void template_free(TemplateFile *f) {
    if (!f)
        return;

    TemplateEngine *e = f->owner;
    template_lock(e);

    f->useCount--;

    if (!f->permanent && f->useCount < 1)
        template_free_internal(f);

    template_unlock(e);
}
