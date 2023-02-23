cmake_minimum_required(VERSION 2.8)
project(app)

set(APP_VERSION_MAJOR 1)
set(APP_VERSION_MINOR 0)

option(USE_MY_COMPILER
    "Use Provided compiler implementation" OFF)
option(USE_DB "Use USE_DB implementation" ON)

configure_file(
    "${PROJECT_SOURCE_DIR}/config.h.in"
    "${PROJECT_SOURCE_DIR}/config.h"
)
if (DEFINED USE_MY_COMPILER)
    message("use my compiler")
    include_directories("${PROJECT_SOURCE_DIR}/compiler")
    add_subdirectory("compiler")
    set(EXTRA_LIBS ${EXTRA_LIBS} Compiler)
else()
    message("not use my compiler")
endif()
if (USE_DB)
    message("use USE_DB")
    include_directories("${PROJECT_SOURCE_DIR}/database")
    add_subdirectory("database")
    set(EXTRA_LIBS ${EXTRA_LIBS} database)
else()
    message("not use USE_DB")
endif()

aux_source_directory(. DIR_SRCS)
add_executable(app ${DIR_SRCS})

target_link_libraries(app ${EXTRA_LIBS})
