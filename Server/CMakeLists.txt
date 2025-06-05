add_executable(Server server.cpp
        )

if (WIN32)
    target_link_libraries(Server PRIVATE ws2_32)
endif()

