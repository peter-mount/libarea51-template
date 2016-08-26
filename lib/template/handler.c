#include <microhttpd.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <area51/string.h>
#include <area51/webserver.h>
#include <area51/template.h>
#include "template-int.h"

/* 32k page size */
#define STATIC_PAGE_SIZE 327667

// Don't allow any file begining with . to prevent people trying to scan filesystem
static const char *INVALID = "/.";

/**
 * Callback from microhttpd to read a block from the file
 * 
 * @param cls The FILE to read
 * @param pos The position in the file
 * @param buf Buffer to read into
 * @param max The size of buf
 * @return The number of bytes read into buf
 */
static ssize_t file_reader(void *cls, uint64_t pos, char *buf, size_t max) {
    TemplateFile *f = cls;

    size_t size = f->size - pos;
    if (size > max)
        size = max;
    if (size > 0) {
        memcpy(buf, f->buffer + pos, size);
        return size;
    }

    return 0;
}

/**
 * Callback from microhttpd when it's finished with the file. Here we close it.
 * 
 * @param cls FILE to close
 */
static void free_callback(void *cls) {
    TemplateFile *f = cls;
    if (f)
        template_free(f);
}

/*
 * Respond to a request with the raw content of a template.
 * @param request
 * @param f Template
 * @return MHD_YES if submitted, MHD_NO if not
 */
int template_respondTemplate(WEBSERVER_REQUEST *request, TemplateFile *f) {
    if (!f)
        return MHD_NO;

    // Hand the file to microhttpd to stream back to the client
    struct MHD_Response *response = MHD_create_response_from_callback(f->size, STATIC_PAGE_SIZE, &file_reader, f, &free_callback);
    if (response == NULL) {
        free_callback(f);
        return MHD_NO;
    }

    int ret = MHD_queue_response(webserver_getRequestConnection(request), MHD_HTTP_OK, response);
    MHD_destroy_response(response);

    return ret;
}

/**
 * Handles static content under /var/www. This handles checks for attempts by the client to gain access outside of
 * that directory, then hands the file to microhttpd to stream it back.
 * 
 * @param connection
 * @return 
 */
static int handler(WEBSERVER_REQUEST *request) {

    const char *url = webserver_getRequestUrl(request);
    // Validate url - i.e. must start with /
    if (url[0] != '/')
        return MHD_NO;

    // Validate url - cannot contain /. (also mean /.. as well)
    // This prevents people from scanning outside of /var/www using ../../ style url's
    char *p = findString((char *) url, (char *) INVALID);
    if (p)
        return MHD_NO;

    TemplateEngine *e = webserver_getUserData(request);
    TemplateFile *f = template_get(e, (char *) url);

    if (!f)
        return MHD_NO;

    return template_respondTemplate(request, f);
}

WEBSERVER_HANDLER *template_addHandler_r(WEBSERVER *webserver, char *p, TemplateEngine *e) {
    return webserver_add_handler(webserver, p, handler, e);
}

WEBSERVER_HANDLER *template_addHandler(WEBSERVER *webserver, TemplateEngine *e) {
    return template_addHandler_r(webserver, "/*", e);
}