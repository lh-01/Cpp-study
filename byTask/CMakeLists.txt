cmake_minimum_required(VERSION 3.26)
project(byTask)

set(CMAKE_CXX_STANDARD 17)

add_executable(byTask
        task1.cpp
        task2.cpp
        task2_client.cpp
        task2_server.cpp
)

target_include_directories(byTask PRIVATE include)

if (WIN32)
    target_link_libraries(byTask ws2_32)
endif()