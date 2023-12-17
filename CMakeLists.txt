project(FBackupper)

cmake_minimum_required(VERSION 3.12)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(include)

file(GLOB SOURCES "src/*.cpp")
add_compile_options(-O2 -g -fpermissive -w)

add_executable(FBackupper ${SOURCES})