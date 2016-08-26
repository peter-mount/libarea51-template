/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   template-int.h
 * Author: peter
 *
 * Created on 24 August 2016, 20:54
 */

#ifndef TEMPLATE_INT_H
#define TEMPLATE_INT_H

#include <stdbool.h>
#include <pthread.h>
#include <area51/hashmap.h>
#include <area51/template.h>

#ifdef __cplusplus
extern "C" {
#endif

    struct TemplateEngine {
        // Hashmap containing our templates
        Hashmap *templates;
        // Base directory for disk based templates
        char *baseDir;
        // Mutex for thread safety
        pthread_mutex_t mutex;
    };

    struct TemplateFile {
        // TemplateEngine this template belongs to
        TemplateEngine *owner;
        // Key in template engine, null means it's not reusable
        char *key;
        void (*freeKey)(void *);
        // File descriptor. If -1 then not mmap
        int fd;
        // Position in memory of template
        void *buffer;
        // template size
        size_t size;
        // If set method used to free pos
        void (*freePos)(void *);
        // In use count (not used if permanent)
        unsigned int useCount;
        // if set this template will never be released
        unsigned int permanent : 1;
        // If set this template should be parsed
        unsigned int template : 1;
        // Set when the template is mmap'd
        unsigned int memmapped : 1;
    };

    extern int verbose;

    extern void template_free_internal(TemplateFile *);
    extern int template_load_internal(TemplateEngine *, char *);

    extern int template_lock(TemplateEngine *);
    extern int template_unlock(TemplateEngine *);

#ifdef __cplusplus
}
#endif

#endif /* TEMPLATE_INT_H */

