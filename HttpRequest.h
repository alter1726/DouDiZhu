#pragma once
#include "Buffer.h"
#include "HttpResponse.h"

// 请求头键值对
struct RequestHeader
{
    char *key;
    char *value;
};

// 当前的解析状态
enum HttpRequestState
{
    ParseReqLine,
    ParseReqHeaders,
    ParseReqBody,
    ParseReqDone
};
// 定义http请求结构体
struct HttpRequest
{
    char *method;
    char *url;
    char *version;
    struct RequestHeader *reqHeaders;
    int reqHeadersNum;
    enum HttpRequestState curState;
};

// 初始化
struct HttpRequest *httpRequestInit();
// 重置
void httpRequestReset(struct HttpRequest *req);