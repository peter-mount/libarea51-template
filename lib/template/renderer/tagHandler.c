
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <area51/charbuffer.h>
#include <area51/template.h>
#include "../template-int.h"

#include <area51/trace.h>

static void includeTemplate(TemplateRenderer *r, char *n) {
    if (!n || !n[0])
        return;

    TemplateFile *f = NULL;
    if (n[0] == '/')
        f = r->lookup(n, r->ctx);
    else
        f = webserver_getRequestAttribute(r->request, n);

    if (f) {
        template_render_template(r, f);
        template_free(f);
    }
}

static void requestAttr(TemplateRenderer *r, char *n) {
    void *v = webserver_getRequestAttribute(r->request, n);
    if (v)
        charbuffer_append(r->buffer, v);
}

void renderer_handleTag(TemplateRenderer *r, char *tag) {
    char *sep = strchr(tag, ':');
    if (sep) {
        *sep++ = 0;

        if (strcmp(tag, "request") == 0)
            return requestAttr(r, sep);
        else if (strcmp(tag, "template") == 0)
            return includeTemplate(r, sep);
    } else
        // No : so request attribute
        requestAttr(r, tag);
}
