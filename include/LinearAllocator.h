#ifndef SIMPLE_MEMORY_MANAGER_LINEAR_ALLOCATOR_GUARD
#define SIMPLE_MEMORY_MANAGER_LINEAR_ALLOCATOR_GUARD

#include "IAllocator.h"

namespace SimpleMemoryAllocator {

	/**
	* A custom allocator that allocates memory in a linear fashion. Being the simplest, and therefore
	* fastest allocator. It cannot deallocate its memory randomly, but rather has to clear it completely.
	*/
	class LinearAllocator : public IAllocator {
	private:
		void* m_firstFree;

		void* __allocate(size_t, uint8_t);
		void __deallocate(void*);
	public:
		LinearAllocator(size_t);
		LinearAllocator(void*, size_t);
		~LinearAllocator();

		void clear();
	};

}

#endif