project(qxikeywatcher)
cmake_minimum_required(VERSION 3.5.0)

find_package(
    Qt5
    COMPONENTS
        Widgets
    REQUIRED
)
find_package(
    X11
    COMPONENTS
        Xext
        Xi
        Xkb
    REQUIRED
)

set(CMAKE_AUTOMOC ON)
set(MOCUIC_DIR ${CMAKE_CURRENT_BINARY_DIR}/.mocuic)

add_executable(
    qxikeywatcher
        demo/demo.cpp
        src/qxikeywatcher.cpp
)

target_include_directories(
    qxikeywatcher
    PRIVATE
        src/
)

set_target_properties(
    qxikeywatcher
    PROPERTIES
        AUTOGEN_BUILD_DIR
            ${MOCUIC_DIR}
)

target_link_libraries(
    qxikeywatcher
        Qt5::Widgets
        X11
        Xi
)
