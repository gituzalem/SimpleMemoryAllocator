#ifndef SIMPLE_MEMORY_MANAGER_POOL_ALLOCATOR_GUARD
#define SIMPLE_MEMORY_MANAGER_POOL_ALLOCATOR_GUARD

#include "IAllocator.h"

namespace SimpleMemoryAllocator {

	/**
	* A custom allocator working as a "pool" of equally sized objects. Since the size of the
	* objects is constant and known in advance, this reduces the memory and processing overhead.
	* Also negates the bad effect of memory fragmentation, sice every time a block is freed, it is 
	* guaranteed to be able to hold another one in its place.
	*/
	class PoolAllocator : public IAllocator {
	private:
		void** m_freeList;
		size_t m_objectSize;
		uint8_t m_objectAlignment;

		void* __allocate(size_t, uint8_t);
		void __deallocate(void*);
	public:
		PoolAllocator(void*, size_t, size_t, uint8_t);
		virtual ~PoolAllocator();
	};

}

#endif