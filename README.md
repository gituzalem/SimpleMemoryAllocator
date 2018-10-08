# SimpleMemoryAllocator

A collection of simple custom memory allocators for C++. A small personal project of mine, striving mainly for simplicity of use and user feature completeness.
It's a project on which wish to learn practical skills, improve upon it and eventually use it in future projects.

This library is licenced under the MIT Licence.


### USAGE
Just `#include "SimpleMemoryAllocator.h"` from the `includes` folder and link against `-lSimpleMemoryAllocator` (you can choose from both static and dynamic linking) and you're set! Now you just need to pick an allocator, initialize it, and use it to manage your heap allocation/deallocations much quicker than a native C++ new/delete.

There are currently these allocators implemented:
  - `LinearAllocator` - An allocator that allocates memory linearly from the beginning. Being the simplest, and therefore the fastest allocator. It cannot deallocate its memory randomly, but rather has to clear it all at once. 

  - `PoolAllocator`   - An allocator working as a pool/array of equally sized objects. Since the size of the objects is constant and known in advance, this reduces the memory and processing overhead. Also negates the bad effect of memory fragmentation, sice every time a block is freed, the allocator is guaranteed to be able to hold another one in its place.

  - `StackAllocator`  - An allocator working like a stack. Therefore, all allocated memory must be deallocated in a LIFO fashion: if you allocated A first and then B, you must first deallocate B before deallocating A.
                        
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
  
  Type* arr = poolAllocator.allocate_array<Type>(arraySize);
  
  // ... do something ...
  
  poolAllocator.deallocate_array(arr);
  
  // ...
```


### HOW TO BUILD
Create a build directory (e.g. `mkdir build`) in the root directory, enter it (e.g. `cd build`) and from there, invoke CMake with the parent folder as the argument (e.g. `cmake ..`). This will generate all the necessary build files (Makefile for Unix, VS solution for MSVC) for your current platform, from which you can build the libraries and examples, which will be located in `output` directory of the root directory.


### FUTURE PLANS (roughly sorted by descending priority)
  - create/add a logger
  - add (more) examples and benchmarks
  - more allocators (Free list, Buddy, ...)
  - reallocation support for easier developing/debugging
  - new features? time will tell...