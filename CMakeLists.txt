cmake_minimum_required(VERSION 3.18.0)
project (DiffFinder)

find_package(PkgConfig REQUIRED)
find_package( OpenCV REQUIRED )

pkg_check_modules(GTK3 REQUIRED gtk+-3.0)

include_directories(${GTK3_INCLUDE_DIRS})
link_directories(${GTK3_LIBRARY_DIRS})
include_directories( ${OpenCV_INCLUDE_DIRS} )
include_directories( ${CMAKE_PROJECT_NAME} "include")

add_definitions(${GTK3_CFLAGS_OTHER})

add_subdirectory(source)
aux_source_directory(. ALL_SRCS)

add_executable(${CMAKE_PROJECT_NAME} ${ALL_SRCS})

target_link_libraries( ${CMAKE_PROJECT_NAME} ${OpenCV_LIBS} ${GTK3_LIBRARIES} DiffFinderImlp)