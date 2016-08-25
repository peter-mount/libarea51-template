
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include "template-int.h"

pthread_mutex_t template_mutex;

int template_lock() {
    return pthread_mutex_lock(&template_mutex);
}

int template_unlock() {
    pthread_mutex_unlock(&template_mutex);
}