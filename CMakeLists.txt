cmake_minimum_required(VERSION 3.22)

project(ThreadTool VERSION 0.1.0 LANGUAGES C)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

add_executable(thtool)
add_library(thlib STATIC)

target_sources(
    thtool
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/main.c
)
target_sources(
    thlib
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/lib/arghandler.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/lib/threads.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/lib/rundata.c
)
target_include_directories(
    thlib
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(
    thtool
    PRIVATE
        thlib
)
