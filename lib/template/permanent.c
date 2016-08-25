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

/**
 * Mark a TemplateFile as permanent - i.e. it will not be freed once all users
 * have finished with it but will remain in memory for reuse later
 * @param f
 */
void template_setPermanent(TemplateEngine *e, TemplateFile *f) {
    if (!f)
        return;

    template_lock(e);
    f->permanent = true;
    template_unlock(e);
}

/**
 * Mark a TemplateFile as transient (default). Once transient then once all users
 * have finished with it then the instance will be freed.
 * @param f
 * 
 *
 */
void template_clearPermanent(TemplateEngine *e, TemplateFile *f) {
    if (!f)
        return;

    template_lock(e);
    f->permanent = false;
    template_unlock(e);
}

/**
 * Loads a TemplateFile from disk and mark it as permanent.
 * 
 * Useful for pre-loading common templates when known.
 * @param n
 * @return EXIT_SUCCESS when loaded
 */
int template_loadPermanent(TemplateEngine *e, char *n) {
    int r = EXIT_FAILURE;

    if (n && n[0]) {
        template_lock(e);

        if (!template_load_internal(e, n)) {
            TemplateFile *f = hashmapGet(template_hashmap, n);
            if (f) {
                f->permanent = true;
                r = EXIT_SUCCESS;
            }
        }

        template_unlock(e);
    }
    return r;
}
