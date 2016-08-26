
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include "template-int.h"

int template_lock(TemplateEngine *e) {
    return e ? pthread_mutex_lock(&e->mutex) : EXIT_SUCCESS;
}

int template_unlock(TemplateEngine *e) {
    return e ? pthread_mutex_unlock(&e->mutex) : EXIT_SUCCESS;
}