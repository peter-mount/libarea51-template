#include <stdlib.h>
#include <string.h>
#include <area51/template.h>
#include "../template-int.h"

/**
 * Find start of substitution {{ }} block.
 * @param f
 * @param o start point
 * @return position after {{ or -1 if not found
 */
int renderer_findStart(TemplateFile *f, int o) {
    // Look for {{, but also must have terminating }} so -3 as look ahead
    int max = f->size - 3;
    if (max < 3)
        return -1;
    char *b = f->buffer;
    while (o < max) {
        // Find {{ return position after match
        if (b[o] == '{' && b[o + 1] == '{')
            return o + 2;
        o++;
    }
    return -1;
}

/**
 * Find end of substitution {{ }} block.
 * @param f
 * @param o start point
 * @return position of }} or -1 if not found
 */
int renderer_findEnd(TemplateFile *f, int o) {
    // Lookahead for }}
    int max = f->size - 1;
    if (max < 3)
        return -1;
    char *b = f->buffer;
    while (o < max) {
        if (b[o] == '}' && b[o + 1] == '}')
            return o;
        o++;
    }
    return -1;
}
