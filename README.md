# SimpleMemoryAllocator

A simple custom memory allocator in C++. A small personal project of mine, striving mainly for simplicity of use and user feature completeness.
It's a project on which wish to learn practical skills, improve upon it and eventually use it in future projects.

This library is licenced under the MIT Licence.

### FUTURE PLANS (roughly sorted by descending priority)
  - create/add a logger
  - add (more) examples and benchmarks
  - add a system to not allow deallocating an array with an single object deallocate() and vice versa
  - make allocators thread-safe
  - make the MSVC solution for potential Windows users
  - extend the Makefile to create a dynamic library as well
  - implement more allocators
  - new features? time will tell...
  

### CHANGELOG
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
