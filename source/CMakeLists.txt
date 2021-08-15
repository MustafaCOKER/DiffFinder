include_directories( ${OpenCV_INCLUDE_DIRS} )

include_directories(
    ${PROJECT_SOURCE_DIR}/include
)

aux_source_directory(. LIB_SRCS)
add_library (DiffFinderImlp STATIC ${LIB_SRCS})

target_link_libraries(DiffFinderImlp ${OpenCV_LIBS})