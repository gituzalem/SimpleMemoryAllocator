#include "../include/PoolAllocator.h"
#include "../include/Util.h"

using namespace SimpleMemoryAllocator;

template <class T>
PoolAllocator::PoolAllocator(size_t poolSize) : PoolAllocator(nullptr, poolSize*sizeof(T) + alignof(T), sizeof(T), alignof(T)) { }

template <class T>
PoolAllocator::PoolAllocator(void* memoryPtr, size_t poolSize) : PoolAllocator(memoryPtr, poolSize*sizeof(T) + alignof(T), sizeof(T), alignof(T)) { }

PoolAllocator::PoolAllocator(size_t memorySize, size_t objectSize, uint8_t objectAlignment) : PoolAllocator(nullptr, memorySize, objectSize, objectAlignment) { }

PoolAllocator::PoolAllocator(void* memoryPtr, size_t memorySize, size_t objectSize, uint8_t objectAlignment) : IAllocator(memoryPtr, memorySize) {
	if (memoryPtr == nullptr)
		memoryPtr = m_start;

	uint8_t adjustment = MemoryUtils::getNextAddressAdjustment(memoryPtr, objectAlignment);

	// align only at memoryPtr, this should make the rest automatically aligned
	m_freeList = (void**)MemoryUtils::addToPointer(memoryPtr, adjustment);

	// initialize the free cell linked list
	// each node free node n initially points to node n+1 and the last node points to null
	size_t numObjects = (memorySize - adjustment) / objectSize;
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