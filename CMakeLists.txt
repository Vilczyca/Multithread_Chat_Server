set(CMAKE_PREFIX_PATH "C:/Qt/6.9.1/mingw_64/lib/cmake")
#set(CMAKE_PREFIX_PATH "D:/Qt/6.9.1/mingw_64/lib/cmake")

cmake_minimum_required(VERSION 3.28.1)
project(Multithread_Chat_Server LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Qt
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)


add_subdirectory(Client)
add_subdirectory(Server)



