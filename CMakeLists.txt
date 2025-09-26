cmake_minimum_required(VERSION 3.16)
project(MyProject LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

file(GLOB SOURCES src/*.cpp)

add_executable(simple_server
  src/simple_server.cpp
)

add_executable(multicast_server
  src/multicast_server.cpp
)

target_include_directories(simple_server PRIVATE include)
target_compile_options(simple_server PRIVATE -Wall -Wextra -g)

target_include_directories(multicast_server PRIVATE include)
target_compile_options(multicast_server PRIVATE -Wall -Wextra -g)
