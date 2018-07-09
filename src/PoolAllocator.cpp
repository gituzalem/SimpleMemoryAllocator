#include "../include/PoolAllocator.h"
#include "../include/Util.h"

using namespace SimpleMemoryAllocator;

PoolAllocator::PoolAllocator(void* start, size_t size, size_t objectSize, uint8_t objectAlignment) : IAllocator(start, size) {
	uint8_t adjustment = MemoryUtils::getNextAddressAdjustment(start, objectAlignment);

	// align only at start, this will make the rest automatically aligned
	m_freeList = (void**)MemoryUtils::addToPointer(start, adjustment);

	// initialize the free cell linked list
	// each node free node n initially points to node n+1 and the last node points to null
	size_t numObjects = (size - adjustment) / objectSize;
	void** ptr = m_freeList;
	for (size_t i = 0; i < numObjects - 1; ++i) {
		*ptr = MemoryUtils::addToPointer(ptr, objectSize);
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
	m_usedMemory += size;
	++m_numAllocations;

	return ptr;
}

void PoolAllocator::__deallocate(void* ptr) {
	throw_assert(ptr != nullptr, "deallocated pointer must not be null");

	*((void**)ptr) = m_freeList;
	m_freeList = (void**)ptr;
	m_usedMemory -= m_objectSize;
	--m_numAllocations;
}