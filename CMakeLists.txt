cmake_minimum_required(VERSION 3.24)
project(Reactor_Http C)

# 设置 C 语言标准
set(CMAKE_C_STANDARD 99)

# 包含头文件路径
include_directories(
    ${PROJECT_SOURCE_DIR}
)

# 列出所有源文件
set(SOURCES
    Buffer.c
    Channel.c
    ChannelMap.c
    EpollDispatcher.c
    EventLoop.c
    HttpRequest.c
    HttpResponse.c
    PollDispatcher.c
    SelectDispatcher.c
    TcpConnection.c
    TcpServer.c
    ThreadPool.c
    WorkerThread.c
    main.c
)

# 添加可执行文件
add_executable(reactor_http ${SOURCES})

# 设置可执行文件输出路径
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
