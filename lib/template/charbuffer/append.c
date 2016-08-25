
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <area51/charbuffer.h>
#include <area51/template.h>

#include "../template-int.h"

int template_appendCharbuffer(CharBuffer *b, TemplateFile *f) {
    if(!f)
        return CHARBUFFER_ERROR;
    
    return charbuffer_put(b, f->buffer, (int) f->size);
}
