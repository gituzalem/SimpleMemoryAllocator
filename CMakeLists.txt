cmake_minimum_required(VERSION 3.0)
project(SimpleMemoryAllocator)

set(EXECUTABLE_OUTPUT_PATH ../output/)
set(LIBRARY_OUTPUT_PATH    ../output/)

set(CXX_FLAGS -Wall -O3 -ggdb)

add_library(memutils 
    src/MemUtils)
target_include_directories(memutils PRIVATE 
    include/)
target_compile_options(memutils PRIVATE 
    "${CXX_FLAGS}")

add_library(simplememoryallocator SHARED
    src/LinearAllocator
    src/PoolAllocator
    src/StackAllocator)
add_dependencies(simplememoryallocator 
    memutils)
target_link_libraries(simplememoryallocator 
    memutils)
target_include_directories(simplememoryallocator PRIVATE 
    include/)
target_compile_options(simplememoryallocator PRIVATE 
    "${CXX_FLAGS}")

add_executable(linear_allocator_example 
    examples/LinearAllocatorExample.cpp)
target_link_libraries(linear_allocator_example 
    simplememoryallocator)
add_dependencies(linear_allocator_example 
    simplememoryallocator)
target_include_directories(linear_allocator_example PRIVATE 
    include/)
target_compile_options(linear_allocator_example PRIVATE 
    "${CXX_FLAGS}")
