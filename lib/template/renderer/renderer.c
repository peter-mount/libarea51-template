
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <area51/charbuffer.h>
#include <area51/template.h>
#include "../template-int.h"

void template_render_template(TemplateRenderer *r, TemplateFile *f) {
    int o = 0;
    while (o>-1) {
        int s = renderer_findStart(f, o);
        if (s < 0)
            break;

        int e = renderer_findEnd(f, s);
        // {{ but no }} so thats it, 
        if (e > s) {
            // Include any text up to insertion -2 as {{
            int l = s - o - 2;
            if (s > 1 && l > 0)
                charbuffer_put(r->buffer, &((char *) f->buffer)[o], s - o - 2);

            // get the tag content
            l = e - s + 1;
            char tag[l];
            memset(tag, 0, l);
            memcpy(tag, &((char *) f->buffer)[s], l - 1);
            tag[l - 1] = 0;

            // process the tag
            renderer_handleTag(r, tag);

            // Move to next position, +2 for }}
            o = e + 2;
        }
    }

    if (o > -1)
        charbuffer_put(r->buffer, &((char *) f->buffer)[o], f->size - o);
}

CharBuffer * template_render(WEBSERVER_REQUEST *r, TemplateFile *(*lookup)(char *, void *), void *ctx, TemplateFile * f) {
    TemplateRenderer renderer;
    memset(&renderer, 0, sizeof (struct TemplateRenderer));

    renderer.buffer = charbuffer_new();
    if (!renderer.buffer)
        return NULL;

    renderer.request = r;
    renderer.lookup = lookup;
    renderer.ctx = ctx;

    template_render_template(&renderer, f);

    return renderer.buffer;
}
