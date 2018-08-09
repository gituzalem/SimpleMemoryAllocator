cmake_minimum_required(VERSION 3.0)
project(SimpleMemoryAllocator)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=c++17")

set(EXECUTABLE_OUTPUT_PATH ../output/)
set(LIBRARY_OUTPUT_PATH    ../output/)

add_library(mem_utils        src/mem_utils/MemUtils)

add_library(linear_allocator src/linear_allocator/LinearAllocator)
target_link_libraries(linear_allocator mem_utils)
add_dependencies(linear_allocator mem_utils)

add_library(pool_allocator   src/pool_allocator/PoolAllocator)
target_link_libraries(pool_allocator mem_utils)
add_dependencies(pool_allocator mem_utils)

add_library(stack_allocator  src/stack_allocator/StackAllocator)
target_link_libraries(stack_allocator mem_utils)
add_dependencies(stack_allocator mem_utils)

add_executable(linear_allocator_example examples/LinearAllocatorExample.cpp)
target_link_libraries(linear_allocator_example linear_allocator)
add_dependencies(linear_allocator_example linear_allocator)