cmake_minimum_required(VERSION 3.0)
project(SimpleMemoryAllocator)

set(EXECUTABLE_OUTPUT_PATH ../output/)
set(LIBRARY_OUTPUT_PATH    ../output/)

set(CXX_FLAGS -Wterminate -I../include/ -O3 -ggdb)

add_library(memutils src/MemUtils)
target_compile_options(memutils PRIVATE "${CXX_FLAGS}")

add_library(simplememoryallocator 
    src/LinearAllocator
    src/PoolAllocator
    src/StackAllocator)
target_link_libraries(simplememoryallocator memutils)
add_dependencies(simplememoryallocator memutils)
target_compile_options(simplememoryallocator PRIVATE "${CXX_FLAGS}")

add_executable(linear_allocator_example examples/LinearAllocatorExample.cpp)
target_link_libraries(linear_allocator_example simplememoryallocator)
add_dependencies(linear_allocator_example simplememoryallocator)
target_compile_options(linear_allocator_example PRIVATE "${CXX_FLAGS}")
