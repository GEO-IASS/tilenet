

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin/samples)
add_definitions(-DTILENET_SAMPLES)

add_subdirectory(connect-four)
add_subdirectory(multi-explore)
add_subdirectory(test)