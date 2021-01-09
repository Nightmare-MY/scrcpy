cmake_minimum_required(VERSION 3.13)
project(NiScrcpy)

set(CMAKE_CXX_STANDARD 14)

include_directories(/usr/local/Cellar/ffmpeg/4.3.1_4/include/)
include_directories(./)
link_directories(/usr/local/Cellar/ffmpeg/4.3.1_4/lib/)

include_directories(/usr/local/Cellar/sdl2/2.0.12_1/include/)
link_directories(/usr/local/Cellar/sdl2/2.0.12_1/lib)

aux_source_directory(. SRC_LIST) # 搜索当前目录下的所有.cpp文件


add_executable(NiScrcpy ${SRC_LIST} ./util/str_util.c  ./util/net.c ./sys/unix/command.c )

target_link_libraries(
        NiScrcpy
        avcodec
        avdevice
        avfilter
        avformat
        avresample
        avutil
        postproc
        swresample
        swscale
        SDL2
)