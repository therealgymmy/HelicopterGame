#ifndef EVENTS_H
#define EVENTS_H

#include <pthread.h>

#include "resources.h"
#include "x_handler.h"

struct EventsArg {
    Resources *resources_;
    XHandler  *xhandler_;
    pthread_mutex_t *mutex_;
};

void* events (void *controlArg);

#endif//EVENTS_H
