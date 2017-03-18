cmake_minimum_required(VERSION 2.6)
project(ppc-to-llvm)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(translation/include /usr/local/opt/capstone/include googletest/googletest/include googletest/googletest)
link_directories(/usr/local/opt/capstone/lib)

add_library(translation translation/src/disassembly.cpp translation/src/translation.cpp)

add_executable(tests tests/main.cpp tests/translation.cpp googletest/googletest/src/gtest-all.cc)
target_link_libraries(tests translation capstone)

set(TRANSLATION_TEST_DATA_DIR ${CMAKE_CURRENT_SOURCE_DIR}/tests/data/translation)

foreach(test blr)
add_custom_target(${test}_bin DEPENDS ${TRANSLATION_TEST_DATA_DIR}/${test}.bin)
add_custom_command(OUTPUT ${test}.o COMMAND /usr/local/opt/llvm/bin/llvm-mc -arch ppc64 -filetype obj ${TRANSLATION_TEST_DATA_DIR}/${test}.s -o ${test}.o MAIN_DEPENDENCY ${TRANSLATION_TEST_DATA_DIR}/${test}.s)
add_custom_command(OUTPUT ${TRANSLATION_TEST_DATA_DIR}/${test}.bin COMMAND gobjcopy --strip-all --output-target binary ${test}.o ${TRANSLATION_TEST_DATA_DIR}/${test}.bin MAIN_DEPENDENCY ${test}.o)
add_dependencies(tests ${test}_bin)
endforeach(test)
