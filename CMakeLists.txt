project(qxikeywatcher)
cmake_minimum_required(VERSION 3.5.0)

find_package(
    Qt5
    REQUIRED
        Widgets
)

find_package(
    X11
    COMPONENTS
        Xext
        Xi
        Xkb
)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(MOCUIC_DIR ${CMAKE_CURRENT_BINARY_DIR}/.mocuic)
set(QXIKEYWATCHER_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR})

add_executable(
    qxikeywatcher
        src/main.cpp
        src/qxikeywatcher.cpp
)

set_target_properties(
    qxikeywatcher
    PROPERTIES
        AUTOGEN_BUILD_DIR
            ${MOCUIC_DIR}
        RUNTIME_OUTPUT_DIRECTORY
            ${QXIKEYWATCHER_BINARY_DIR}
)

target_link_libraries(
    qxikeywatcher
        Qt5::Widgets
        X11
        Xi
)
