#include "Channel.h"

struct Channel *channelInit(int fd, int events, handleFunc readFunc, handleFunc writeFunc, handleFunc destroyFunc, void *arg)
{
    struct Channel *channel = (struct channel *)malloc(sizeof(struct Channel));
    channel->fd = fd;
    channel->events = events;
    channel->readCallback = readFunc;
    channel->writeCallback = writeFunc;
    channel->destroyCallback = destroyFunc;
    channel->arg = arg;
    return channel;
}

void writeEventEnable(struct Channel *channel, bool flag)
{
    if (flag)
    {
        channel->events |= WriteEvent;
    }
    else
    {
        channel->events = channel->events & ~WriteEvent;
    }
}

bool isWriteEventEnable(struct Channel *channel)
{
    return channel->events & WriteEvent;
}
