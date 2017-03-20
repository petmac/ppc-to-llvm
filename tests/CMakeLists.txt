set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(TRANSLATION_TEST_DATA_DIR ${CMAKE_CURRENT_SOURCE_DIR}/data/translation)
set(TESTS blr)

foreach(test ${TESTS})
add_custom_target(${test}_bin DEPENDS ${test}.bin)
add_custom_command(OUTPUT ${test}.o COMMAND /usr/local/opt/llvm/bin/llvm-mc -arch ppc64 -filetype obj ${TRANSLATION_TEST_DATA_DIR}/${test}.s -o ${test}.o MAIN_DEPENDENCY ${TRANSLATION_TEST_DATA_DIR}/${test}.s)
add_custom_command(OUTPUT ${test}.bin COMMAND gobjcopy --strip-all --output-target binary ${test}.o ${test}.bin MAIN_DEPENDENCY ${test}.o)
endforeach(test)

include_directories(../translation/include /usr/local/opt/capstone/include ../googletest/googletest/include ../googletest/googletest)
link_directories(/usr/local/opt/capstone/lib)

add_executable(tests main.cpp translation.cpp ../googletest/googletest/src/gtest-all.cc)
target_link_libraries(tests translation capstone)

foreach(test ${TESTS})
add_dependencies(tests ${test}_bin)
endforeach(test)
