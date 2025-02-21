
#define _GNU_SOURCE
#include "Buffer.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/uio.h>
#include <string.h>

struct Buffer *bufferInit(int size)
{
    struct Buffer *buffer = (struct Buffer *)malloc(sizeof(struct Buffer));
    if (buffer != NULL)
    {
        buffer->data = (char *)malloc(size);
        buffer->capacity = size;
        buffer->writePos = buffer->readPos = 0;
        memset(buffer->data, 0, size);
    }
    return buffer;
}

void bufferDestroy(struct Buffer *buf)
{
    if (buf != NULL)
    {
        if (buf->data != NULL)
        {
            free(buf->data);
        }
    }
    free(buf);
}

void bufferExtendRoom(struct Buffer *buffer, int size)
{
    // 内存够用 - 不需要扩容
    if (bufferWriteableSize(buffer) >= size)
    {
        return;
    }
    // 内存需要合并才够用 - 不需要扩容
    // 剩余的可写的内存 + 已读的内存 > size
    else if (buffer->readPos + bufferWriteableSize(buffer) >= size)
    {
        // 得到未读的内存大小
        int readable = bufferReadableSize(buffer);
        // 移动内存
        memcpy(buffer->data, buffer->data + buffer->readPos, readable);
        // 更新位置
        buffer->readPos = 0;
        buffer->writePos = readable;
    }
    // 内存不够用 - 扩容
    else
    {
        void *temp = realloc(buffer->data, buffer->capacity + size);
        if (temp == NULL)
        {
            return; // 失败
        }
        memset(temp + buffer->capacity, 0, size);
        // 更新数据
        buffer->data = temp;
        buffer->capacity += size;
    }
}

int bufferWriteableSize(struct Buffer *buffer)
{
    return buffer->capacity - buffer->writePos;
}

int bufferReadableSize(struct Buffer *buffer)
{
    return buffer->writePos - buffer->readPos;
}

int bufferAppendData(struct Buffer *buffer, const char *data, int size)
{
    if (buffer == NULL || data == NULL || size <= 0)
    {
        return -1;
    }
    // 扩容
    bufferExtendRoom(buffer, size);
    // 数据拷贝
    memcpy(buffer->data + buffer->writePos, data, size);
    buffer->writePos += size;
    return 0;
}

int bufferAppendString(struct Buffer *buffer, const char *data)
{
    int size = strlen(data);
    int ret = bufferAppendData(buffer, data, size);
    return ret;
}

int bufferSocketRead(struct Buffer *buffer, int fd)
{
    // read/recv/readv
    struct iovec vec[2];
    // 初始化数组元素
    int writeable = bufferWriteableSize(buffer);
    vec[0].iov_base = buffer->data + buffer->writePos;
    vec[0].iov_len = writeable;
    char *tmpbuf = (char *)malloc(40960);
    vec[1].iov_base = tmpbuf;
    vec[1].iov_len = 40960;
    int result = readv(fd, vec, 2);
    if (result == -1)
    {
        return -1;
    }
    else if (result <= writeable)
    {
        buffer->writePos += result;
    }
    else
    {
        buffer->writePos = buffer->capacity;
        bufferAppendData(buffer, tmpbuf, result - writeable);
    }
    free(tmpbuf);
    return result;
}