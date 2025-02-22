#include "TcpConnection.h"
#include <stdlib.h>
#include <stdio.h>

int processRead(void *arg)
{
    struct TcpConnection *conn = (struct TcpConnection *)arg;
    // 接收数据
    int count = bufferSocketRead(conn->readBuf, conn->channel->fd);
    if (count > 0)
    {
        // 接收到了 http 请求, 解析http请求
    }
    else
    {
        // 断开连接
    }
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

int tcpConnectionDestroy(void *arg)
{
    struct TcpConnection *conn = (struct TcpConnection *)arg;
    if (conn != NULL)
    {
        if (conn->readBuf && bufferReadableSize(conn->readBuf) == 0 &&
            conn->writeBuf && bufferReadableSize(conn->writeBuf) == 0)
        {
            destroyChannel(conn->evLoop, conn->channel);
            bufferDestroy(conn->readBuf);
            bufferDestroy(conn->writeBuf);
            free(conn);
        }
    }
    return 0;
}