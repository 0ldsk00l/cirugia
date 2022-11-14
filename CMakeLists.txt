cmake_minimum_required(VERSION 3.16)

project(cirugia VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)
qt_standard_project_setup()

add_executable(cirugia
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    about.cpp
    about.h
    about.ui
    nesrom.cpp
    nesrom.h
    hasher.cpp
    hasher.h
)

target_link_libraries(cirugia PRIVATE Qt6::Widgets)
