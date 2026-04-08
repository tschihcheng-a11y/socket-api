cmake_minimum_required(VERSION 3.10)
project(SocketTest LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 如果從 socket/src 獨立 build，需要找到 SocketLib
if(NOT TARGET SocketLib)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../src ${CMAKE_CURRENT_BINARY_DIR}/SocketLib)
endif()

add_executable(test_server test_server.cpp)
target_link_libraries(test_server PRIVATE SocketLib)

add_executable(test_client test_client.cpp)
target_link_libraries(test_client PRIVATE SocketLib)
