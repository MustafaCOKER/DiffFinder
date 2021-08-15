cmake_minimum_required(VERSION 3.18.0)
project (DiffFinder)

find_package( OpenCV REQUIRED )

include_directories( ${OpenCV_INCLUDE_DIRS} )
include_directories(${CMAKE_PROJECT_NAME} "include")

add_subdirectory(source)
aux_source_directory(. ALL_SRCS)

add_executable(${CMAKE_PROJECT_NAME} ${ALL_SRCS})

target_link_libraries( ${CMAKE_PROJECT_NAME} ${OpenCV_LIBS} DiffFinderImlp)