#pragma once
#include "EventLoop.h"
#include "Buffer.h"
#include "Channel.h"

struct TcpConnection
{
    struct EventLoop *evLoop;
    struct Channel *channel;
    struct Buffer *readBuf;
    struct Buffer *writeBuf;
    char name[32];
};

// 初始化
struct TcpConnection *tcpConnectionInit(int fd, struct EventLoop *evloop);
int tcpConnectionDestroy(void *conn);