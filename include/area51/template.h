/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   template.h
 * Author: peter
 *
 * Created on 24 August 2016, 20:04
 */

#ifndef TEMPLATE_H
#define TEMPLATE_H

#include <area51/charbuffer.h>
#include <area51/webserver.h>
#include <microhttpd.h>

#ifdef __cplusplus
extern "C" {
#endif

    /*
     * Handle on a template file
     */
    typedef struct TemplateFile TemplateFile;

    extern int template_init(char *);

    extern void template_clearPermanent(TemplateFile *);
    extern void template_setPermanent(TemplateFile *);
    extern int template_loadPermanent(char *);

    TemplateFile *template_new(void *, size_t , void (*)(void *));
    extern TemplateFile *template_get(char *);
    extern void template_free(TemplateFile *);

    extern int template_appendCharbuffer(CharBuffer *, TemplateFile *);

    extern int templateHandler(struct MHD_Connection *, WEBSERVER_HANDLER *handler, const char *);

#ifdef __cplusplus
}
#endif

#endif /* TEMPLATE_H */

