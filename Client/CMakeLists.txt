add_executable(Client client.cpp)

if (WIN32)
    target_link_libraries(Client PRIVATE ws2_32)
    target_include_directories(Client PRIVATE ../chat)
endif()
