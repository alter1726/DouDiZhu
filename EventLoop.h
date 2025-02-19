#pragma once
#include <stdbool.h>
#include "Dispatcher.h"

extern struct Dispatcher EpollDispatcher;
extern struct Dispatcher PollDispatcher;
extern struct Dispatcher SelectDispatcher;

struct EventLoop
{
    bool isQuit;
    struct Dispatcher *dispatcher;
    void *dispatcherData;
};