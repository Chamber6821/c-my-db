cmake_minimum_required(VERSION 3.23)
project(c-my-db C)

set(CMAKE_C_STANDARD 17)

add_subdirectory(../forks/c-vector c-vector)
function(linkLibraries)
    target_link_libraries(${ARGV0} c-vector)
endfunction()

file(GLOB_RECURSE SOURCES src/*.c)

add_executable(${PROJECT_NAME} ${SOURCES} main.c)
target_include_directories(${PROJECT_NAME} PUBLIC include)
cmake_language(CALL linkLibraries ${PROJECT_NAME})

enable_testing()
file(GLOB_RECURSE TESTS tests/*.c)
foreach(TEST_FILE ${TESTS})
    file(RELATIVE_PATH CASE_NAME "${PROJECT_SOURCE_DIR}/tests/" ${TEST_FILE})
    string(REPLACE "/" "_" CASE_NAME ${CASE_NAME})
    set(TEST_NAME "TEST_${CASE_NAME}")

    add_executable(${TEST_NAME} ${TEST_FILE} ${SOURCES})
    target_include_directories(${TEST_NAME} PUBLIC include)
    cmake_language(CALL linkLibraries ${TEST_NAME})

    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
endforeach()
