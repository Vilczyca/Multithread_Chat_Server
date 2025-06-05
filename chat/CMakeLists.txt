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
        userwidget.cpp
        userwidget.h
        userwidget.ui
)

add_library(chat STATIC ${CHAT_SOURCES})

target_link_libraries(chat PUBLIC Qt${QT_VERSION_MAJOR}::Widgets)

target_include_directories(chat PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
