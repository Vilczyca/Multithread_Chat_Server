cmake_minimum_required(VERSION 3.30)
project(multithreaded_chat_server)

set(CMAKE_CXX_STANDARD 20)

add_executable(multithreaded_chat_server server.cpp)

if (WIN32)
    target_link_libraries(multithreaded_chat_server ws2_32)
endif()