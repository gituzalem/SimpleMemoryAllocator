# SimpleMemoryAllocator

A collection of simple custom memory allocators for C++. A small personal project of mine, striving mainly for simplicity of use and user feature completeness.
It's a project on which wish to learn practical skills, improve upon it and eventually use it in future projects.

This library is licenced under the MIT Licence.


### USAGE
Just `#include "SimpleMemoryAllocator.h"` from the `includes` folder and link against `-lSimpleMemoryAllocator` (you can choose from both static and dynamic linking) and you're set! Now you just need to pick an allocator, initialize it, and use it to manage your heap allocation/deallocations much quicker than a native C++ new/delete.

There are currently these allocators implemented:
  - `LinearAllocator` - An allocator that allocates memory in a linear fashion. Being the simplest, and therefore 
                        the fastest allocator. It cannot deallocate its memory randomly, but rather has to clear it completely. 

  - `PoolAllocator`   - An allocator working as a "pool" of equally sized objects. Since the size of the
	                      objects is constant and known in advance, this reduces the memory and processing overhead.
	                      Also negates the bad effect of memory fragmentation, sice every time a block is freed, it is 
	                      guaranteed to be able to hold another one in its place.

  - `StackAllocator`  - An allocator working like a stack. Therefore, all allocated memory must be deallocated
	                      in a LIFO fashion: if you allocated A first and then B, you must first deallocate B before 
	                      deallocating A.
                        
A simple usage example: 
```C++
#include "SimpleMemoryAllocator.h"

int main() {
  size_t memorySize = 4096;
  size_t objectSize = sizeof(Type);
  uint8_t objectAlignment = alignof(Type);
  
  SimpleMemoryAllocator::PoolAllocator poolAllocator(memorySize, objectSize, objectAlignment);
  
  Type* p = poolAllocator.allocate<Type>();
  
  // ... do something ...
  
  poolAllocator.deallocate(p);
  
  return 0;
}
```
In this case, memory new/delete is handled by the allocator. But you may want to manage such allocation yourself and have the allocators work on a previously allocated memory. You can do that by providing a `void` pointer as a first argument to the allocator constructor:
```C++
  size_t memorySize = ...;
  void* memoryPointer = ::operator new(memorySize);

  // ...

  SimpleMemoryAllocator::LinearAllocator linearAllocator(memoryPointer, memorySize);

  // ...

  ::operator delete(memoryPointer);
```
You can also allocate/deallocate arrays like this:
```C++
  // ...

  SimpleMemoryAllocator::PoolAllocator poolAllocator<Type>(numObjects);
  
  Type* arr = poolAllocator.allocateArray<Type>(arraySize);
  
  // ... do something ...
  
  poolAllocator.deallocateArray(arr);
  
  // ...
```

### HOW TO BUILD
Create a build directory (e.g. `mkdir build`) in the root directory, enter it (e.g. `cd build`) and from there, invoke CMake with the parent folder as the argument (e.g. `cmake ..`). This will generate all the necessary build files (Makefile for Unix, VS solution for MSVC) for your current platform, from which you can build the libraries and examples, which will be located in `output` directory of the root directory.


### FUTURE PLANS (roughly sorted by descending priority)
  - create/add a logger
  - add (more) examples and benchmarks
  - add a system to not allow deallocating an array with an single object deallocate() and vice versa
  - reallocation support for easier debugging
  - add the option to make allocators thread-safe
  - more allocators
  - new features? time will tell...
  

### CHANGELOG
v0.2.2
  - changed project structure
  - switched from Makefile to CMake as build system

v0.2.1
  - expanded readme
  - source files and Makefile cosmetic changes
  - restructured build dir
  - you can now also build a dynamic library with Makefile
  - added a more intuitive constructor for PoolAllocator
  - provided documentation for IAllocator, PoolAllocator, LinearAllocator classes and MemoryUtils namespace and functions

v0.2
  - changed general allocate/deallocate interface functions from free functions to IAllocator member functions
  - added ::operator new memory allocation/deallocation if memory is not already provided for constructor

v0.1.2
  - added the ability to allocate and deallocate arrays
  - added an assertion system based on exceptions
  - added a LinearAllocator example with personal thoughts on code
  - various minor fixes
  
v0.1.1 
  - rebranded to SimpleMemoryAllocator to better reflect its funcionality
  - added this readme and changelog
  - changed licence to MIT
  - removed MSVC solution and project, will be readded once it works

v0.1 
  - initial GitHub commit as SimpleMemoryManager!
