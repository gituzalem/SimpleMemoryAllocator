#include "../include/SimpleMemoryAllocator.h"
#include <iostream>
#include <cstring>
#include <ctime>

struct TestStruct {
	int field1;
	char field2;
	bool field3;
};


std::clock_t g_firstTime;

void startTimer() {
	g_firstTime = std::clock();
}

int endTimer() {
	std::clock_t secondTime = std::clock();
	int ret = (secondTime - g_firstTime);
	g_firstTime = secondTime;

	return ret;
}

void _printSection(const char* a_sectionName, char a_horizontalChar = '-', int a_bottomLineLength = 80) {
	const int nameSize = strlen(a_sectionName);

	// print tab top
	std::cout << "\n+" << a_horizontalChar;
	for(int i = 0; i < nameSize; ++i) std::cout << a_horizontalChar;
	std::cout << a_horizontalChar << "+\n";

	std::cout << "| " << a_sectionName << " |\n";

	// print tab bottom
	std::cout << "+" << a_horizontalChar;
	for(int i = 0; i < nameSize; ++i) std::cout << a_horizontalChar;
	std::cout << a_horizontalChar << "+";
	for(int i = 0; i < (a_bottomLineLength - nameSize - 4); ++i) std::cout << a_horizontalChar;
	std::cout << "\n";
}

void printMainSection(const char* a_sectionName, int a_bottomLineLength = 80) {
	_printSection(a_sectionName, '=', a_bottomLineLength);
}

void printSubSection(const char* a_sectionName, int a_bottomLineLength = 80) {
	_printSection(a_sectionName, '-', a_bottomLineLength);
}

int main(int argc, char** argv) {

	const size_t memorySize = 4096*4096;
	const size_t arraySize = 42;

	//////////////////////
	// BASIC OPERATIONS //
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	printMainSection("BASIC OPERATIONS");

	// allocate a chunk of working memory with new
	startTimer();
	void* startPtr = ::operator new(memorySize);                                                        // probably add a way to do memory allocations in the constructor
	                                                                                                    // if no pointer is provided to reduce boilerplate
	std::cout << "::operator new allocated " << memorySize << " bytes in " << endTimer() << " clicks\n";

	// instantiate a linear allocator
	SimpleMemoryAllocator::LinearAllocator linearAllocator(startPtr, memorySize);

	// allocate a single element
	startTimer();
	TestStruct* ts = linearAllocator.allocate<TestStruct>();											// this looks so much better than SimpleMemoryAllocator::allocate(allocator, size)
	if (ts == nullptr) {                                                                                
		std::cout << "couldn't allocate a single object, wtf\n";
	}
	std::cout << "allocated a single item in " << endTimer() << " clicks, there are " << linearAllocator.getNumAllocations() << " allocations and " << linearAllocator.getUsedMemory() << " bytes of memory used\n";

	// allocate an array of elements
	startTimer();
	TestStruct* tsa = linearAllocator.allocateArray<TestStruct>(arraySize);
	std::cout << "allocated an array of " << arraySize << " items in " << endTimer() << " clicks, there are " << linearAllocator.getNumAllocations() << " allocations and " << linearAllocator.getUsedMemory() << " bytes of memory used\n";


	// try deallocate and catch the exception it throws
	try {
		linearAllocator.deallocateArray(tsa);
	} catch (SimpleMemoryAllocator::AssertException e) {
		std::cout << "AssertException caught: " << e.what() << "\n";
	}

	// clear the allocator
	startTimer();
	linearAllocator.clear();
	std::cout << "linear allocator cleared in " << endTimer() << " clicks, there are " << linearAllocator.getNumAllocations() << " allocations and " << linearAllocator.getUsedMemory() << " bytes of memory used\n";

	// free the working memory
	::operator delete(startPtr);                                                                        // also free the memory in the allocator if it was 
	                                                                                                    // allocated inside
	                         
	// allocator-managed base memory allocation
	SimpleMemoryAllocator::LinearAllocator secondLinearAllocator(memorySize);

	startTimer();
	ts = secondLinearAllocator.allocateArray<TestStruct>(arraySize);
	if (ts == nullptr) {                                                                                
		std::cout << "couldn't allocate an array of " << arraySize << " elements\n";
	}
	std::cout << "allocated a second single item in " << endTimer() << " clicks, there are " << secondLinearAllocator.getNumAllocations() << " allocations and " << secondLinearAllocator.getUsedMemory() << " bytes of memory used\n";

	secondLinearAllocator.clear();

	///////////////////////
	// BENCHMARK VS. NEW //
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	printMainSection("BENCHMARK");

	const int TEST_ARRAY_SIZE = 200000;

	TestStruct* arr[TEST_ARRAY_SIZE];

	printSubSection("NATIVE NEW");
	startTimer();
	for(int i = 0; i < TEST_ARRAY_SIZE; ++i) {
		arr[i] = new TestStruct;
	}
	std::cout << "C++ native new: allocated " << TEST_ARRAY_SIZE << " pointers to TestStruct in " << endTimer() << " ms.\n";

	startTimer();
	for(int i = 0; i < TEST_ARRAY_SIZE; ++i) {
		delete arr[i];
	}
	std::cout << "C++ native new: deallocated " << TEST_ARRAY_SIZE << " pointers to TestStruct in " << endTimer() << " ms.\n";


	printSubSection("LINEAR ALLOCATOR");

	startTimer();
	SimpleMemoryAllocator::LinearAllocator benchmarkLinearAllocator(TEST_ARRAY_SIZE * sizeof(TestStruct));
	std::cout << "LinearAllocator: allocated memory for " << TEST_ARRAY_SIZE << " pointers to TestStruct in " << endTimer() << " ms.\n";

	startTimer();
	for(int i = 0; i < TEST_ARRAY_SIZE; ++i) {
		arr[i] = benchmarkLinearAllocator.allocate<TestStruct>();
	}
	std::cout << "LinearAllocator: allocated " << TEST_ARRAY_SIZE << " pointers to TestStruct in " << endTimer() << " ms.\n";

	startTimer();
	benchmarkLinearAllocator.clear();
	std::cout << "LinearAllocator: deallocated " << TEST_ARRAY_SIZE << " pointers to TestStruct in " << endTimer() << " ms.\n";



	return 0;
}