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

#ifdef __cplusplus
extern "C" {
#endif

    typedef struct TemplateEngine TemplateEngine;
    typedef struct TemplateFile TemplateFile;

    extern TemplateEngine *template_init(char *);

    extern void template_clearPermanent(TemplateEngine *, TemplateFile *);
    extern void template_setPermanent(TemplateEngine *, TemplateFile *);
    extern int template_loadPermanent(TemplateEngine *, char *);

    TemplateFile *template_new(void *, size_t, void (*)(void *));
    extern TemplateFile *template_get(TemplateEngine *, char *);
    extern void template_free(TemplateEngine *, TemplateFile *);

    extern int template_appendCharbuffer(CharBuffer *, TemplateFile *);

    extern WEBSERVER_HANDLER *template_addHandler(WEBSERVER *, TemplateEngine *);

#ifdef __cplusplus
}
#endif

#endif /* TEMPLATE_H */

