#ifndef SIMPLE_MEMORY_MANAGER_STACK_ALLOCATOR_GUARD
#define SIMPLE_MEMORY_MANAGER_STACK_ALLOCATOR_GUARD

#include "IAllocator.h"

namespace SimpleMemoryManager {

	/**
	* A custom allocator working in a stack fashion. Therefore, all allocated memory must be deallocated
	* in a LIFO fashion: if you allocated A first and then B, you must first deallocate B before 
	* deallocating A.
	*/
	class StackAllocator : public IAllocator {
	private:
		void* m_top;
		void* m_previousTop;
	public:
		StackAllocator(void*, size_t);
		virtual ~StackAllocator();

		void* allocate(size_t, uint8_t);
		void deallocate(void*);
	};

	struct StackAllocationHeader {
		uint8_t adjustment;
	};

}

#endif