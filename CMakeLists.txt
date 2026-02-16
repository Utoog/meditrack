cmake_minimum_required(VERSION 3.20)
project(MediTrack)

file(GLOB_RECURSE SOURCES src/*.c)

include_directories(include)

add_executable(meditrack ${SOURCES})

find_package(SQLite3 REQUIRED)

target_link_libraries(meditrack PUBLIC sqlite3)

set(CPACK_PACKAGE_NAME "MediTrack")
set(CPACK_PACKAGE_VERSION "0.0.1")

set(CPACK_MONOLITHIC_INSTALL 1)

include (CPack)