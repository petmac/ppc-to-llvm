cmake_minimum_required(VERSION 2.6)
project(ppc-to-llvm)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(translation/include /usr/local/opt/capstone/include googletest/googletest/include googletest/googletest)
link_directories(/usr/local/opt/capstone/lib)

add_library(translation translation/src/disassembly.cpp translation/src/translation.cpp)

add_executable(tests tests/main.cpp tests/translation.cpp googletest/googletest/src/gtest-all.cc)
target_link_libraries(tests translation capstone)
add_custom_command(TARGET tests POST_BUILD COMMAND make WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/tests/data/translation)
