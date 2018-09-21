#include "PoolAllocator.h"

using namespace SimpleMemoryAllocator;

template <class T>
PoolAllocator::PoolAllocator(size_t pool_size) : PoolAllocator(nullptr, pool_size*sizeof(T) + alignof(T), sizeof(T), alignof(T)) { }

template <class T>
PoolAllocator::PoolAllocator(void* memory_ptr, size_t pool_size) : PoolAllocator(memory_ptr, pool_size*sizeof(T) + alignof(T), sizeof(T), alignof(T)) { }

PoolAllocator::PoolAllocator(size_t memory_size, size_t objectSize, uint8_t object_alignment) : PoolAllocator(nullptr, memory_size, objectSize, object_alignment) { }

PoolAllocator::PoolAllocator(void* memory_ptr, size_t memory_size, size_t objectSize, uint8_t object_alignment) : IAllocator(memory_ptr, memory_size) {
	if (memory_ptr == nullptr)
		memory_ptr = m_start;

	uint8_t adjustment = MemoryUtils::get_next_address_adjustment(memory_ptr, object_alignment);

	// align only at memory_ptr, this should make the rest automatically aligned
	m_freeList = (void**)MemoryUtils::add_to_pointer(memory_ptr, adjustment);

	// initialize the free cell linked list
	// each node free node n initially points to node n+1 and the last node points to null
	size_t numObjects = (memory_size - adjustment) / objectSize;
	void** ptr = m_freeList;
	for (size_t i = 0; i < numObjects - 1; ++i) {
		*ptr = MemoryUtils::add_to_pointer(ptr, objectSize);
		ptr = (void**) *ptr;
	}
	*ptr = nullptr;
}

PoolAllocator::~PoolAllocator() {
	m_freeList = nullptr;
}

void* PoolAllocator::__allocate(size_t size, uint8_t alignment) {
	throw_assert(size > 0, "allocated size must be larger than 0");

	// return null pointer if there are no more cells left
	if (m_freeList == nullptr) return nullptr;

	void* ptr = m_freeList;				// get first free block
	m_freeList = (void**)(*m_freeList);	// and then set the next free block as the first free block
	m_used_memory += size;
	++m_num_allocations;

	return ptr;
}

void PoolAllocator::__deallocate(void* ptr) {
	throw_assert(ptr != nullptr, "deallocated pointer must not be null");

	*((void**)ptr) = m_freeList;
	m_freeList = (void**)ptr;
	m_used_memory -= m_objectSize;
	--m_num_allocations;
}