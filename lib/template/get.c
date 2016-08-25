#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <area51/hashmap.h>
#include <area51/template.h>

#include "template-int.h"

TemplateFile *template_get(char *name) {
    template_lock();

    if (!hashmapContainsKey(template_hashmap, name))
        if (template_load_internal(name)) {
            template_unlock();
            return NULL;
        }

    TemplateFile *f = hashmapGet(template_hashmap, name);

    if (f && !f->permanent)
        f->useCount++;

    template_unlock();
    return f;
}
