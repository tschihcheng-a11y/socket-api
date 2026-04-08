cmake_minimum_required(VERSION 3.10)
project(SocketLib VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Threads REQUIRED)

add_library(SocketLib STATIC
    client/SocketClent.cpp
    server/SockeServer.cpp
)

target_include_directories(SocketLib
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/client>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/server>
        $<INSTALL_INTERFACE:include>
)

target_link_libraries(SocketLib
    PRIVATE
        Threads::Threads
        $<$<PLATFORM_ID:Windows>:ws2_32>
)

install(TARGETS SocketLib
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
)

install(FILES
    spec.h
    client/ISocketClent.h
    client/SocketClient.h
    server/ISocketServer.h
    server/SocketServer.h
    DESTINATION include/SocketLib
)
