
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

    bool lookup = n[0] == '/';
    TemplateFile *f = NULL;
    if (lookup)
        f = r->lookup(n, r->ctx);
    else {
        void *v = webserver_getRequestAttribute(r->request, n);
        // If value starts with / then it's a template name, else a template
        if (v && ((char *) v)[0] == '/') {
            f = r->lookup((char *) v, r->ctx);
            lookup = true;
        } else
            f = v;
    }

    if (f) {
        template_render_template(r, f);

        // If we used lookup to get it, free it
        if (lookup)
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
