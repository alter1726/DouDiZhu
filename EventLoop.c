#include "EventLoop.h"

struct EventLoop *eventLoopInit()
{
    return eventLoopInitEx(NULL);
}

// 写数据
void taskWakeup(struct EventLoop *evLoop)
{
    const char *msg = "我要成为牛马!!!";
    write(evLoop->socketPair[0], msg, strlen(msg));
}

// 读数据
int readLocalMessage(void *arg)
{
    struct EventLoop *evLoop = (struct EventLoop *)arg;
    char buf[256];
    read(evLoop->socketPair[1], buf, sizeof(buf));
    return 0;
}

struct EventLoop *eventLoopInitEx(const char *threadName)
{
    struct EventLoop *evLoop = (struct EventLoop *)malloc(sizeof(struct EventLoop));
    evLoop->isQuit = false;
    evLoop->threadID = pthread_self();
    pthread_mutex_init(&evLoop->mutex, NULL);
    strcpy(evLoop->threadName, threadName == NULL ? "MainThread" : threadName);
    evLoop->dispatcher = &SelectDispatcher;
    evLoop->dispatcherData = evLoop->dispatcher->init();
    // 链表
    evLoop->head = evLoop->tail = NULL;
    // map
    evLoop->channelMap = channelMapInit(128);
    return evLoop;
}

int eventLoopRun(struct EventLoop *evLoop)
{
    assert(evLoop != NULL);
    // 取出事件分发和检测模型
    struct Dispatcher *dispatcher = evLoop->dispatcher;
    // 比较线程ID是否正常
    if (evLoop->threadID != pthread_self())
    {
        return -1;
    }
    // 循环进行事件处理
    while (!evLoop->isQuit)
    {
        dispatcher->dispatch(evLoop, 2); // 超时时长2s
        eventLoopProcessTask(evLoop);
    }
    return 0;
}

int eventActivate(struct EventLoop *evLoop, int fd, int event)
{
    if (fd < 0 || evLoop == NULL)
    {
        return -1;
    }
    // 取出channel
    struct Channel *channel = evLoop->channelMap->list[fd];
    assert(channel->fd == fd);
    if (event & ReadEvent && channel->readCallback)
    {
        channel->readCallback(channel->arg);
    }
    if (event & WriteEvent && channel->writeCallback)
    {
        channel->writeCallback(channel->arg);
    }
    return 0;
}

int eventLoopAddTask(struct EventLoop *evLoop, struct Channel *channel, int type)
{
    // 加锁,保护共享资源
    pthread_mutex_lock(&evLoop->mutex);
    // 创建新节点
    struct ChannelElement *node = (struct ChannelElement *)malloc(sizeof(struct ChannelElement));
    node->channel = channel;
    node->type = type;
    node->next = NULL;
    // 链表为空
    if (evLoop->head == NULL)
    {
        evLoop->head = evLoop->tail = node;
    }
    else
    {
        evLoop->tail->next = node; // add
        evLoop->tail = node;       // 后移
    }
    pthread_mutex_unlock(&evLoop->mutex);
    // 处理节点
    if (evLoop->threadID == pthread_self())
    {
        // 当前子线程
        eventLoopProcessTask(evLoop);
    }
    else
    {
        // 主线程 -- 告诉子线程处理任务队列中的任务
        // 1. 子线程在工作 2. 子线程被阻塞了:select, poll, epoll
        taskWakeup(evLoop);
    }
    return 0;
}

int eventLoopProcessTask(struct EventLoop *evLoop)
{
    pthread_mutex_lock(&evLoop->mutex);
    // 取出头结点
    struct ChannelElement *head = evLoop->head;
    while (head != NULL)
    {
        struct Channel *channel = head->channel;
        if (head->type == ADD)
        {
            // 添加
            eventLoopAdd(evLoop, channel);
        }
        else if (head->type == DELETE)
        {
            // 删除
            eventLoopRemove(evLoop, channel);
        }
        else if (head->type == MODIFY)
        {
            // 修改
            eventLoopModify(evLoop, channel);
        }
        struct ChannelElement *tmp = head;
        head = head->next;
        free(tmp);
    }
    evLoop->head = evLoop->tail = NULL;
    pthread_mutex_unlock(&evLoop->mutex);
    return 0;
}

int eventLoopAdd(struct EventLoop *evLoop, struct Channel *channel)
{
    int fd = channel->fd;
    struct ChannelMap *channelMap = evLoop->channelMap;
    if (fd >= channelMap->size)
    {
        // 没有足够的空间存储键值对fd - channel -> 扩容
        if (!makeMapRoom(channelMap, fd, sizeof(struct Channel *)))
        {
            return -1;
        }
    }
    // 找到fd对应的数组元素位置,并存储
    if (channelMap->list[fd] == NULL)
    {
        channelMap->list[fd] = channel;
        evLoop->dispatcher->add(channel, evLoop);
    }
    return 0;
}

int eventLoopRemove(struct EventLoop *evLoop, struct Channel *channel)
{
    int fd = channel->fd;
    struct ChannelMap *channelMap = evLoop->channelMap;
    if (fd >= channelMap->size)
    {
        return -1;
    }
    int ret = evLoop->dispatcher->remove(channel, evLoop);
    return ret;
}

int eventLoopModify(struct EventLoop *evLoop, struct Channel *channel)
{
    int fd = channel->fd;
    struct ChannelMap *channelMap = evLoop->channelMap;
    if (channelMap->list[fd] == NULL)
    {
        return -1;
    }
    int ret = evLoop->dispatcher->modify(channel, evLoop);
    return ret;
}

int destroyChannel(struct EventLoop *evLoop, struct Channel *channel)
{
    // 删除channel和fd的对应关系
    evLoop->channelMap->list[channel->fd] = NULL;
    // 关闭fd
    close(channel->fd);
    // 释放channel
    free(channel);
    return 0;
}