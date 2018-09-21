cmake_minimum_required(VERSION 3.0)
project(SimpleMemoryAllocator)

set(CMAKE_BUILD_TYPE RELEASE)
set(CMAKE_CXX_FLAGS, "${CMAKE_CXX_FLAGS} -Wterminate")
set(CMAKE_CXX_FLAGS_DEBUG, "${CMAKE_CXX_FLAGS_DEBUG} -O0 -ggdb -DDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE, "${CMAKE_CXX_FLAGS_RELEASE} -O2")

set(EXECUTABLE_OUTPUT_PATH ../output/)
set(LIBRARY_OUTPUT_PATH    ../output/)

add_library(memutils src/mem_utils/MemUtils)

add_library(simplememoryallocator 
    src/linear_allocator/LinearAllocator
    src/pool_allocator/PoolAllocator
    src/stack_allocator/StackAllocator)
target_link_libraries(simplememoryallocator memutils)
add_dependencies(simplememoryallocator memutils)

add_executable(linear_allocator_example examples/LinearAllocatorExample.cpp)
target_link_libraries(linear_allocator_example simplememoryallocator)
add_dependencies(linear_allocator_example simplememoryallocator)
