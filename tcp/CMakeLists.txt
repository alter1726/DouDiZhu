cmake_minimum_required(VERSION 3.24)
project(tcp)
set(CMAKE_C_STANDARD 99)

# 取出当前目录中所有的源文件, 存储到一个变量中
aux_source_directory(./ SRC)
# 把源文件制作成静态库
add_library(tcp STATIC ${SRC})