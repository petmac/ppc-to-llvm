set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(../include /usr/local/opt/capstone/include)
link_directories(/usr/local/opt/capstone/lib)

set(src_files disassembly.cpp disassembly.h translation.cpp ../include/ppc-to-llvm/translation.h)

add_library(translation ${src_files})
