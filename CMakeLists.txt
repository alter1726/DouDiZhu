cmake_minimum_required(VERSION 3.24)
project(Reactor_Http C)

# 设置 C 语言标准
set(CMAKE_C_STANDARD 99)

# 添加头文件路径
include_directories(
        ${PROJECT_SOURCE_DIR}/common
        ${PROJECT_SOURCE_DIR}/http
        ${PROJECT_SOURCE_DIR}/reactor
        ${PROJECT_SOURCE_DIR}/tcp
        ${PROJECT_SOURCE_DIR}/thread
)

# 添加子目录
add_subdirectory(common)
add_subdirectory(http)
add_subdirectory(reactor)
add_subdirectory(tcp)
add_subdirectory(thread)

# 链接子目录中生成的静态库文件
link_libraries(tcp http reactor thread common)
# 添加可执行文件
add_executable(reactor_http main.c)

# 设置可执行文件输出路径
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
