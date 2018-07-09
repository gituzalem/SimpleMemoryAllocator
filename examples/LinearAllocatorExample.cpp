#include "SimpleMemoryAllocator.h"
#include <iostream>

struct TestStruct {
	int field1;
	char field2;
	bool field3;
};


int main(int argc, char** argv) {

	const size_t memorySize = 4096;
	const size_t arraySize = 42;
	
	// allocate a chunk of working memory with new
	void* startPtr = ::operator new(memorySize);                                                        // probably add a way to do memory allocations in the constructor
                                                                                                        // if no pointer is provided to reduce boilerplate
	// instantiate a linear allocator
	SimpleMemoryAllocator::LinearAllocator linearAllocator(startPtr, memorySize);

	// allocate a single element
	TestStruct* ts = SimpleMemoryAllocator::allocate<TestStruct>(linearAllocator);                      // I already don't like this syntax, probably change it
	if (ts != nullptr) {                                                                                // to allocator.allocate<class T>(size) syntax soon
		ts->field1 = 42;
		ts->field2 = 'a';
		ts->field3 = true;
	} else {
		std::cout << "";
	}

	// allocate an array of elements
	TestStruct* tsa = SimpleMemoryAllocator::allocateArray<TestStruct>(linearAllocator, arraySize);

	for (size_t i = 0; i < arraySize; ++i) {
		std::cout << "tsa[" << i << "].field1 == " << tsa[i].field1 << "\n";
		std::cout << "tsa[" << i << "].field2 == " << tsa[i].field2 << "\n";
		std::cout << "tsa[" << i << "].field3 == " << tsa[i].field3 << "\n";
	}

	// try deallocate and catch the exception it throws
	try {
		SimpleMemoryAllocator::deallocateArray(linearAllocator, tsa);
	} catch (SimpleMemoryAllocator::AssertException e) {
		std::cout << "AssertException caught: " << e.what() << "\n";
	}

	// clear the allocator
	linearAllocator.clear();                                                                            // this syntax looks much clearer than the current one

	// free the working memory
	::operator delete(startPtr);                                                                        // also free the memory in the allocator if it was 
                                                                                                        // allocated inside
	return 0;
}