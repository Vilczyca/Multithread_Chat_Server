#set(CMAKE_PREFIX_PATH "C:/Qt/6.9.1/mingw_64/lib/cmake")
#set(CMAKE_PREFIX_PATH "D:/Qt/6.9.1/mingw_64/lib/cmake")

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# Zbierz pliki GUI
set(CHAT_SOURCES
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        userwidget.cpp
        userwidget.h
        userwidget.ui
)

# Combine all source files for the executable
set(CLIENT_SOURCES
        client.cpp
        ${CHAT_SOURCES}
)

# Create just one target - the executable
add_executable(Client ${CLIENT_SOURCES})

target_link_libraries(Client PUBLIC Qt${QT_VERSION_MAJOR}::Widgets)

target_include_directories(Client PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

if (WIN32)
    target_link_libraries(Client PRIVATE ws2_32)
endif()