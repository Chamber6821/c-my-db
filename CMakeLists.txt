cmake_minimum_required(VERSION 3.23)
project(c-my-db C)

set(CMAKE_C_STANDARD 17)

file(GLOB_RECURSE SOURCES src/*.c)
add_executable(${PROJECT_NAME} ${SOURCES} main.c)
target_include_directories(${PROJECT_NAME} PUBLIC include)


enable_testing()
file(GLOB_RECURSE TESTS tests/*.c)
foreach(TEST_FILE ${TESTS})
    file(RELATIVE_PATH CASE_NAME "${PROJECT_SOURCE_DIR}/tests/" ${TEST_FILE})
    string(REPLACE "/" "_" CASE_NAME ${CASE_NAME})
    set(TEST_NAME "TEST_${CASE_NAME}")

    add_executable(${TEST_NAME} ${TEST_FILE})
    target_include_directories(${TEST_NAME} PUBLIC include)
    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
endforeach()
