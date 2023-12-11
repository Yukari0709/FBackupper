project(FBackupper)

cmake_minimum_required(VERSION 3.12)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(include)

file(GLOB SOURCES "src/*.cpp")

add_executable(FBackupper ${SOURCES})