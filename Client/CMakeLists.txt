add_executable(Client client.cpp)

if (WIN32)
    target_link_libraries(Client ws2_32)
endif()