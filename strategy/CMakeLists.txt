cmake_minimum_required(VERSION 3.16)
project(strategy)

#搜索当前目录下的所有源文件
aux_source_directory(./ SRC)

file(GLOB HEADERS ${PROJECT_SOURCE_DIR}/*.h)

#将当前目录下的源文件制作成静态库
add_library(strategy ${SRC} ${HEADERS})
target_link_libraries(strategy PRIVATE Qt${QT_VERSION_MAJOR}::Core)
