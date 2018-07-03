#include "../include/StackAllocator.h"
#include "../include/Util.h"

using namespace SimpleMemoryAllocator;

StackAllocator::StackAllocator(void* start, size_t size) : IAllocator(start, size), m_top(m_start), m_previousTop(nullptr) {  }

StackAllocator::~StackAllocator() {
	m_top = nullptr;
	m_previousTop = nullptr;
}

void* StackAllocator::allocate(size_t size, uint8_t alignment) {
	throw_assert(size > 0, "allocated size must be larger than 0");

	uint8_t adjustment = MemoryUtils::getNextAddressAdjustmentWithHeader(m_top, alignment, sizeof(StackAllocationHeader));

	// don't allocate if we need to allocate more than we have free
	if (m_usedMemory + size + adjustment > m_size) return nullptr;

	void* alignedAddress = MemoryUtils::addToPointer(m_top, adjustment);

	// store allocation header before the actual stored data
	StackAllocationHeader* header = (StackAllocationHeader*)MemoryUtils::addToPointer(alignedAddress, -sizeof(StackAllocationHeader));
	header->adjustment = adjustment;

	m_previousTop = m_top;
	m_top = MemoryUtils::addToPointer(alignedAddress, size);
	m_usedMemory += size + adjustment;
	++m_numAllocations;

	return (void*)alignedAddress;
}

void StackAllocator::deallocate(void* ptr) {
	throw_assert(ptr != nullptr, "deallocated pointer must not be null");

	StackAllocationHeader* header = (StackAllocationHeader*)MemoryUtils::addToPointer(ptr, -sizeof(StackAllocationHeader));
	m_usedMemory -= ((unsigned*)m_top - (unsigned*)ptr + header->adjustment);
	m_top = MemoryUtils::addToPointer(ptr, header->adjustment);
	--m_numAllocations;
}