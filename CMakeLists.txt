cmake_minimum_required(VERSION 4.0)
project(solution)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

#set(SANITIZER_FLAGS "-fsanitize=address,undefined")
#set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${SANITIZER_FLAGS} -fno-omit-frame-pointer")

add_executable(gentests gentests.cpp)
add_executable(solution solution.cpp)

include(CTest)
add_executable(testrunner tests/testrunner.cpp)

file(GLOB IN_FILES "${CMAKE_SOURCE_DIR}/tests/in/*.in")

foreach(f IN LISTS IN_FILES)
    get_filename_component(dir  "${f}" DIRECTORY)
    get_filename_component(base "${f}" NAME_WE)

    if (EXISTS "${dir}/${base}.txt")
        file(REMOVE "${dir}/${base}.txt")
    endif()

    file(RENAME "${f}" "${dir}/${base}.txt")
endforeach()

file(GLOB OUT_FILES "${CMAKE_SOURCE_DIR}/tests/out/*.out")

foreach(f IN LISTS OUT_FILES)
    get_filename_component(dir  "${f}" DIRECTORY)
    get_filename_component(base "${f}" NAME_WE)

    if (EXISTS "${dir}/${base}.txt")
        file(REMOVE "${dir}/${base}.txt")
    endif()

    file(RENAME "${f}" "${dir}/${base}.txt")
endforeach()

file(GLOB TEST_INPUTS "${CMAKE_SOURCE_DIR}/tests/in/*")
list(SORT TEST_INPUTS COMPARE NATURAL)

foreach(infile ${TEST_INPUTS})
    get_filename_component(case_name "${infile}" NAME)
    set(expected "${CMAKE_SOURCE_DIR}/tests/out/${case_name}")
    add_test(NAME ${case_name}
            COMMAND $<TARGET_FILE:testrunner> $<TARGET_FILE:solution> "${infile}" "${expected}"
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
endforeach()
