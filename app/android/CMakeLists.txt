cmake_minimum_required (VERSION 2.6) # cmake version

project(term) # project name
include_directories(/usr/local/Cellar/ffmpeg/4.3.1_4/include/)
include_directories(./)
link_directories(/usr/local/Cellar/ffmpeg/4.3.1_4/lib/)

include_directories(/usr/local/Cellar/sdl2/2.0.12_1/include/)
link_directories(/usr/local/Cellar/sdl2/2.0.12_1/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/../../../android/app/src/main/jniLibs/$ENV{ABI}) # set output path

include_directories(${PROJECT_SOURCE_DIR}/../src)
aux_source_directory(${PROJECT_SOURCE_DIR}/../src SRC_FILES) # scan source code files

add_library(term SHARED ${SRC_FILES}) # add source code files to library, and set build type to dynamic library.
