cmake_minimum_required(VERSION 3.22)
project(c-my-db C)

set(CMAKE_C_STANDARD 17)

add_library(c-vector lib/c-vector/vec.c)
add_library(sds lib/sds/sds.c)
add_library(inih lib/inih/ini.c)

macro(addLibraries TARGET)
    message(STATUS "Target: ${TARGET}")
    target_include_directories(${TARGET} PUBLIC lib)
    target_link_libraries(${TARGET} c-vector)
    target_link_libraries(${TARGET} sds)
    target_link_libraries(${TARGET} inih)
endmacro()

file(GLOB_RECURSE SOURCES src/*.c)

add_executable(${PROJECT_NAME} ${SOURCES} main.c)
target_include_directories(${PROJECT_NAME} PUBLIC include)
addLibraries(${PROJECT_NAME})

enable_testing()
file(GLOB_RECURSE TESTS tests/*.c)
foreach (TEST_FILE ${TESTS})
    file(RELATIVE_PATH CASE_NAME "${PROJECT_SOURCE_DIR}/tests/" ${TEST_FILE})
    string(REPLACE "/" "_" CASE_NAME ${CASE_NAME})
    set(TEST_NAME "TEST_${CASE_NAME}")

    add_executable(${TEST_NAME} ${TEST_FILE} ${SOURCES})
    target_include_directories(${TEST_NAME} PUBLIC include)
    target_compile_options(${TEST_NAME} PRIVATE -D_DEBUG)
    addLibraries(${TEST_NAME})

    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
endforeach ()
