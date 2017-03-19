cmake_minimum_required(VERSION 2.6)
project(ppc-to-llvm)

add_subdirectory(tests)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(translation/include /usr/local/opt/capstone/include)
link_directories(/usr/local/opt/capstone/lib)

add_library(translation translation/src/disassembly.cpp translation/src/translation.cpp)
