#ifndef CONTROL_H
#define CONTROL_H

#include <pthread.h>

#include "resources.h"
#include "x_handler.h"

struct ControlArg {
    Resources *resources_;
    XHandler  *xhandler_;
    pthread_mutex_t *mutex_;
};

void* control (void *controlArg);

#endif//CONTROL_H
