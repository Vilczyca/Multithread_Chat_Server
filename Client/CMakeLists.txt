add_executable(Client client.cpp
        chatClient.cpp
        chatClient.h)

if (WIN32)
    target_link_libraries(Client PRIVATE ws2_32)
endif()

set(SFML_DIR "D:/InteliJ/CppLibs/SFML-2.6.2/lib/cmake/SFML")
find_package(SFML REQUIRED COMPONENTS graphics window system network)

target_link_libraries(Client PRIVATE
        sfml-graphics
        sfml-window
        sfml-system
        sfml-network
)