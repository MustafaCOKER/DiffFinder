cmake_minimum_required(VERSION 3.18.0)
project (DiffFinder)

find_package( PkgConfig REQUIRED )
find_package( OpenCV    REQUIRED )

pkg_check_modules(GTK3 REQUIRED gtk+-3.0)

# find diffFinder implementation
add_subdirectory(source)
set(DIFF_FINDER_LIB DiffFinderImlp)

# for gtk 
add_definitions(${GTK3_CFLAGS_OTHER})
include_directories(${GTK3_INCLUDE_DIRS})
link_directories(${GTK3_LIBRARY_DIRS})

# for opencv
include_directories( ${OpenCV_INCLUDE_DIRS} )

# cli part
set(CLI_OUTPUT "DiffFinder_CLI")
set(CLI_SOURCE "main.cpp")
include_directories( ${CLI_OUTPUT} "include")
add_executable(${CLI_OUTPUT} ${CLI_SOURCE})
target_link_libraries( ${CLI_OUTPUT} ${OpenCV_LIBS} ${DIFF_FINDER_LIB})

# gui part
set(GUI_OUTPUT "DiffFinder_GUI")
set(GUI_SOURCE "main_gui.cpp")
include_directories( ${GUI_OUTPUT} "include")
add_executable(${GUI_OUTPUT} ${GUI_SOURCE})
target_link_libraries( ${GUI_OUTPUT} ${OpenCV_LIBS} ${GTK3_LIBRARIES} ${DIFF_FINDER_LIB})