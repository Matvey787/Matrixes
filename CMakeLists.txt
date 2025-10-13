# cmake -S . -B build && cmake --build build

# ./build/out
# cmake --build build --target clean
# ctest --test-dir build -V
# g++ -g -O2 -fno-omit-frame-pointer your_program.cpp -o your_program
# -DCMAKE_BUILD_TYPE=RelWithDebInfo

cmake_minimum_required(VERSION 3.30)
project(LIRS_Cache CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
find_package(GTest REQUIRED)

include_directories(${CMAKE_SOURCE_DIR}/inc)

set(COMMON_SOURCE_FILES
    src/main.cpp
)

add_executable(out ${COMMON_SOURCE_FILES})
target_include_directories(out PRIVATE ${CMAKE_SOURCE_DIR}/inc)

add_executable(genTest src/generateTest.cpp)
target_include_directories(genTest PRIVATE ${CMAKE_SOURCE_DIR}/inc)

add_executable(matrix_tests tests/matrix_tests.cpp)
target_link_libraries(matrix_tests GTest::gtest GTest::gtest_main)
target_include_directories(matrix_tests PRIVATE ${CMAKE_SOURCE_DIR}/inc)

include(CTest)
enable_testing()

function(create_tests_for_target TARGET_NAME FOLDER_WITH_TESTS)
    file(GLOB TEST_FILES ${FOLDER_WITH_TESTS}/test*.dat)

    if(NOT TEST_FILES)
        message(WARNING "No test files found.")
        return()
    endif()

    foreach(TEST_FILE ${TEST_FILES})
        get_filename_component(TEST_NAME ${TEST_FILE} NAME_WE)
        string(REGEX REPLACE "^test" "" TEST_NUMBER ${TEST_NAME})

        set(ANS_FILE ${FOLDER_WITH_TESTS}/ans${TEST_NUMBER}.dat)

        if(EXISTS ${ANS_FILE})
            file(READ ${ANS_FILE} EXPECTED_OUTPUT)
            string(STRIP "${EXPECTED_OUTPUT}" EXPECTED_OUTPUT)
            
            add_test(NAME ${TARGET_NAME}_test_${TEST_NUMBER}
                COMMAND bash -c "build/${TARGET_NAME} < ${FOLDER_WITH_TESTS}/test${TEST_NUMBER}.dat"
                WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            )
            
            set_tests_properties(${TARGET_NAME}_test_${TEST_NUMBER} PROPERTIES
                PASS_REGULAR_EXPRESSION "^${EXPECTED_OUTPUT}([ \t\n\r]|$)"
            )
        else()
            message(WARNING "Answer file not found for test ${TEST_NUMBER}: ${ANS_FILE}")
        endif()
    endforeach()
endfunction()

create_tests_for_target(out ${CMAKE_SOURCE_DIR}/tests)
