cmake_minimum_required (VERSION 3.0)
project (stack_machine)

add_compile_options(-std=c++17 -g)

include_directories(include)
add_executable(stack_machine src/main.cpp src/gc.cpp src/data_type.cpp src/runtime_type.cpp src/instruction.cpp src/machine.cpp)
