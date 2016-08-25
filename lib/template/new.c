
#include <stdlib.h>
#include <area51/template.h>
#include <string.h>
#include "template-int.h"

TemplateFile *template_new(void *p, size_t size, void (*f)(void *)) {
    TemplateFile *t = malloc(sizeof (struct TemplateFile));
    if (!t)
        return NULL;

    memset(f, 0, sizeof (struct TemplateFile));
    t->fd = -1;

    t->buffer = p;
    t->size = size;
    t->freePos = f;
    
    return t;
}
