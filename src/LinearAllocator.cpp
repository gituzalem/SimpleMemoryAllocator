#include "../include/LinearAllocator.h"
#include "../include/Util.h"

using namespace SimpleMemoryAllocator;

LinearAllocator::LinearAllocator(void* start, size_t size) : IAllocator(start, size), m_firstFree(m_start) { }

LinearAllocator::~LinearAllocator() {
	m_firstFree = nullptr;
}

void* LinearAllocator::allocate(size_t size, uint8_t alignment) {

	uint8_t adjustment = MemoryUtils::getNextAddressAdjustment(m_firstFree, alignment);

	// don't allocate if we need to allocate more than we have free
	if (m_usedMemory + size + adjustment > m_size) return nullptr;

	uintptr_t alignedAddress = (uintptr_t)(m_firstFree) + adjustment;
	m_firstFree = (void*)(size + alignedAddress);
	m_usedMemory += size + adjustment;
	++m_numAllocations;

	return (void*)alignedAddress;
}

void LinearAllocator::deallocate(void* ptr) {

}

void LinearAllocator::clear() {
	m_numAllocations = 0;
	m_usedMemory = 0;
	m_firstFree = m_start;
}