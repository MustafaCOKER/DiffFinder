cmake_minimum_required(VERSION 3.18.0)
project (DiffFinder)

set(CLI_OUTPUT "DiffFinder_CLI")
set(CLI_SOURCE "main.cpp")

set(GUI_OUTPUT "DiffFinder_GUI")
set(GUI_SOURCE "main_gui.cpp")

find_package(PkgConfig REQUIRED)
find_package( OpenCV REQUIRED )

pkg_check_modules(GTK3 REQUIRED gtk+-3.0)

include_directories(${GTK3_INCLUDE_DIRS})
link_directories(${GTK3_LIBRARY_DIRS})
include_directories( ${OpenCV_INCLUDE_DIRS} )

include_directories( ${CLI_OUTPUT} "include")
include_directories( ${GUI_OUTPUT} "include")

add_definitions(${GTK3_CFLAGS_OTHER})

add_subdirectory(source)

add_executable(${CLI_OUTPUT} ${CLI_SOURCE})
add_executable(${GUI_OUTPUT} ${GUI_SOURCE})

target_link_libraries( ${CLI_OUTPUT} ${OpenCV_LIBS} DiffFinderImlp)
target_link_libraries( ${GUI_OUTPUT} ${OpenCV_LIBS} ${GTK3_LIBRARIES} DiffFinderImlp)