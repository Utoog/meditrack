cmake_minimum_required(VERSION 3.20)
project(MediTrack C)

file(GLOB_RECURSE SOURCES src/*.c)

include_directories(include)

add_definitions(-DUSE_COLOR)
add_compile_options(-Wall -Wextra)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions(-DDebug)
endif()

add_executable(meditrack ${SOURCES})

find_package(SQLite3 REQUIRED)

target_link_libraries(meditrack PUBLIC sqlite3)
