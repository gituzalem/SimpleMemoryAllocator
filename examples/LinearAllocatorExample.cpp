#include <SimpleMemoryAllocator.h>
#include <iostream>
#include <cstring>
#include <ctime>

struct TestStruct {
	int field1;
	char field2;
	bool field3;
};


std::clock_t g_first_time;

void start_timer() {
	g_first_time = std::clock();
}

int end_timer() {
	std::clock_t second_time = std::clock();
	int ret = (second_time - g_first_time);
	g_first_time = second_time;

	return ret;
}

void _print_section(const char* a_section_name, char a_horizontal_char = '-', int a_bottom_line_length = 80) {
	const int name_size = strlen(a_section_name);

	// print tab top
	std::cout << "\n+" << a_horizontal_char;
	for(int i = 0; i < name_size; ++i) std::cout << a_horizontal_char;
	std::cout << a_horizontal_char << "+\n";

	std::cout << "| " << a_section_name << " |\n";

	// print tab bottom
	std::cout << "+" << a_horizontal_char;
	for(int i = 0; i < name_size; ++i) std::cout << a_horizontal_char;
	std::cout << a_horizontal_char << "+";
	for(int i = 0; i < (a_bottom_line_length - name_size - 4); ++i) std::cout << a_horizontal_char;
	std::cout << "\n";
}

void print_main_section(const char* a_section_name, int a_bottom_line_length = 80) {
	_print_section(a_section_name, '=', a_bottom_line_length);
}

void print_sub_section(const char* a_section_name, int a_bottom_line_length = 80) {
	_print_section(a_section_name, '-', a_bottom_line_length);
}

int main(int argc, char** argv) {

	const size_t memorySize = 4096*4096;
	const size_t arraySize = 42;

	//////////////////////
	// BASIC OPERATIONS //
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	print_main_section("BASIC OPERATIONS");

	// allocate a chunk of working memory with new
	start_timer();
	void* startPtr = ::operator new(memorySize);                                                        // probably add a way to do memory allocations in the constructor
	                                                                                                    // if no pointer is provided to reduce boilerplate
	std::cout << "::operator new allocated " << memorySize << " bytes in " << end_timer() << " clicks\n";

	// instantiate a linear allocator
	SimpleMemoryAllocator::LinearAllocator linearAllocator(startPtr, memorySize);

	// allocate a single element
	start_timer();
	TestStruct* ts = linearAllocator.allocate<TestStruct>();											// this looks so much better than SimpleMemoryAllocator::allocate(allocator, size)
	if (ts == nullptr) {                                                                                
		std::cout << "couldn't allocate a single object, wtf\n";
	}
	std::cout << "allocated a single item in " << end_timer() << " clicks, there are " << linearAllocator.get_num_allocations() << " allocations and " << linearAllocator.get_used_memory() << " bytes of memory used\n";

	// allocate an array of elements
	start_timer();
	TestStruct* tsa = linearAllocator.allocate_array<TestStruct>(arraySize);
	std::cout << "allocated an array of " << arraySize << " items in " << end_timer() << " clicks, there are " << linearAllocator.get_num_allocations() << " allocations and " << linearAllocator.get_used_memory() << " bytes of memory used\n";


	// try deallocate and catch the exception it throws
	try {
		linearAllocator.deallocate_array(tsa);
	} catch (SimpleMemoryAllocator::AssertException e) {
		std::cout << "AssertException caught: " << e.what() << "\n";
	}

	// clear the allocator
	start_timer();
	linearAllocator.clear();
	std::cout << "linear allocator cleared in " << end_timer() << " clicks, there are " << linearAllocator.get_num_allocations() << " allocations and " << linearAllocator.get_used_memory() << " bytes of memory used\n";

	// free the working memory
	::operator delete(startPtr);                                                                        // also free the memory in the allocator if it was 
	                                                                                                    // allocated inside
	                         
	// allocator-managed base memory allocation
	SimpleMemoryAllocator::LinearAllocator secondLinearAllocator(memorySize);

	start_timer();
	ts = secondLinearAllocator.allocate_array<TestStruct>(arraySize);
	if (ts == nullptr) {                                                                                
		std::cout << "couldn't allocate an array of " << arraySize << " elements\n";
	}
	std::cout << "allocated a second single item in " << end_timer() << " clicks, there are " << secondLinearAllocator.get_num_allocations() << " allocations and " << secondLinearAllocator.get_used_memory() << " bytes of memory used\n";

	secondLinearAllocator.clear();

	///////////////////////
	// BENCHMARK VS. NEW //
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	print_main_section("BENCHMARK");

	const int TEST_ARRAY_SIZE = 200000;

	TestStruct* arr[TEST_ARRAY_SIZE];

	print_sub_section("NATIVE NEW");
	start_timer();
	for(int i = 0; i < TEST_ARRAY_SIZE; ++i) {
		arr[i] = new TestStruct;
	}
	std::cout << "C++ native new: allocated " << TEST_ARRAY_SIZE << " pointers to TestStruct in " << end_timer() << " ms.\n";

	start_timer();
	for(int i = 0; i < TEST_ARRAY_SIZE; ++i) {
		delete arr[i];
	}
	std::cout << "C++ native new: deallocated " << TEST_ARRAY_SIZE << " pointers to TestStruct in " << end_timer() << " ms.\n";


	print_sub_section("LINEAR ALLOCATOR");

	start_timer();
	SimpleMemoryAllocator::LinearAllocator benchmarkLinearAllocator(TEST_ARRAY_SIZE * sizeof(TestStruct));
	std::cout << "LinearAllocator: allocated memory for " << TEST_ARRAY_SIZE << " pointers to TestStruct in " << end_timer() << " ms.\n";

	start_timer();
	for(int i = 0; i < TEST_ARRAY_SIZE; ++i) {
		arr[i] = benchmarkLinearAllocator.allocate<TestStruct>();
	}
	std::cout << "LinearAllocator: allocated " << TEST_ARRAY_SIZE << " pointers to TestStruct in " << end_timer() << " ms.\n";

	start_timer();
	benchmarkLinearAllocator.clear();
	std::cout << "LinearAllocator: deallocated " << TEST_ARRAY_SIZE << " pointers to TestStruct in " << end_timer() << " ms.\n";



	return 0;
}