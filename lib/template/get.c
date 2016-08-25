#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <area51/hashmap.h>
#include <area51/template.h>

#include "template-int.h"

TemplateFile *template_get(TemplateEngine *e, char *name) {
    template_lock(e);

    if (!hashmapContainsKey(e->templates, name))
        if (template_load_internal(e, name)) {
            template_unlock(e);
            return NULL;
        }

    TemplateFile *f = hashmapGet(e->templates, name);

    if (f && !f->permanent)
        f->useCount++;

    template_unlock(e);
    return f;
}
