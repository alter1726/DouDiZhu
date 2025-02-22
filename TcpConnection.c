#include "TcpConnection.h"
#include <stdlib.h>
#include <stdio.h>

int processRead(void *arg)
{
    return 0;
}

struct TcpConnection *tcpConnectionInit(int fd, struct EventLoop *evloop)
{
    struct TcpConnection *conn = (struct TcpConnection *)malloc(sizeof(struct TcpConnection));
    conn->evLoop = evloop;
    conn->readBuf = bufferInit(10240);
    conn->writeBuf = bufferInit(10240);
    sprintf(conn->name, "Connection-%d", fd);
    conn->channel = channelInit(fd, ReadEvent, processRead, NULL, NULL);
    eventLoopAddTask(evloop, conn->channel, ADD);
    return conn;
}