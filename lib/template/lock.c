
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include "template-int.h"

int template_lock(TemplateEngine *e) {
    return pthread_mutex_lock(&e->mutex);
}

int template_unlock(TemplateEngine *e) {
    pthread_mutex_unlock(&e->mutex);
}