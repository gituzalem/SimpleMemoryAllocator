# SimpleMemoryAllocator

A collection of simple custom memory allocators for C++. A small personal project of mine, striving mainly for simplicity of use and user feature completeness.
It's a project on which wish to learn practical skills, improve upon it and eventually use it in future projects.

This library is licenced under the MIT Licence.


### USAGE
Just `#include "SimpleMemoryAllocator.h"` from the `includes` folder and link against `libSimpleMemoryAllocator.a` (or your local machine equivalent, dynamic linking coming in future) and you're set! Now you just need to pick an allocator, initialize it, and use it to manage your heap allocation/deallocations much quicker than a native C++ new/delete.

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
  
  SimpleMemoryAllocator::PoolAllocator poolAllocator(memorySize);
  
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

  SimpleMemoryAllocator::PoolAllocator poolAllocator(memoryPointer, memorySize);

  // ...

  ::operator delete(memoryPointer);
```
You can also allocate/deallocate arrays like this:
```C++
  // ...

  SimpleMemoryAllocator::PoolAllocator poolAllocator(memorySize);
  
  Type* arr = poolAllocator.allocateArray<Type>(arraySize);
  
  // ... do something ...
  
  poolAllocator.deallocateArray(arr);
  
  // ...
```


### FUTURE PLANS (roughly sorted by descending priority)
  - create/add a logger
  - add (more) examples and benchmarks
  - add a system to not allow deallocating an array with an single object deallocate() and vice versa
  - make allocators thread-safe
  - make a VS solution for MSVC users
  - build a dynamic library as well
  - more allocators
  - new features? time will tell...
  

### CHANGELOG
v0.2.1
  - expanded readme

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
