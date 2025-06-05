add_executable(Server server.cpp
        chatserver.cpp
        chatserver.h)

if (WIN32)
    target_link_libraries(Server PRIVATE ws2_32)
endif()

set(SFML_DIR "D:/InteliJ/CppLibs/SFML-2.6.2/lib/cmake/SFML")
find_package(SFML REQUIRED COMPONENTS graphics window system network)

target_link_libraries(Server PRIVATE
        sfml-graphics
        sfml-window
        sfml-system
        sfml-network
)