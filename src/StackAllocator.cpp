#include "../include/StackAllocator.h"
#include "../include/Util.h"

using namespace SimpleMemoryManager;

StackAllocator::StackAllocator(void* start, size_t size) : IAllocator(start, size), m_top(m_start), m_previousTop(nullptr) {  }

StackAllocator::~StackAllocator() {
	m_top = nullptr;
	m_previousTop = nullptr;
}

void* StackAllocator::allocate(size_t size, uint8_t alignment) {
	uint8_t adjustment = MemoryUtils::getNextAddressAdjustmentWithHeader(m_top, alignment, sizeof(StackAllocationHeader));

	// don't allocate if we need to allocate more than we have free
	if (m_usedMemory + size + adjustment > m_size) return nullptr;

	uintptr_t alignedAddress = (uintptr_t)(m_top) + adjustment;

	// store allocation header before the actual stored data
	StackAllocationHeader* header = (StackAllocationHeader*)(alignedAddress - sizeof(StackAllocationHeader));
	header->adjustment = adjustment;

	m_previousTop = m_top;
	m_top = (void*)(alignedAddress + size);
	m_usedMemory += size + adjustment;
	++m_numAllocations;

	return (void*)alignedAddress;
}

void StackAllocator::deallocate(void* ptr) {
	StackAllocationHeader* header = (StackAllocationHeader*)((uintptr_t)ptr - sizeof(StackAllocationHeader));
	m_usedMemory -= ((uintptr_t)m_top - (uintptr_t)ptr + header->adjustment);
	m_top = (void*)((uintptr_t)ptr - header->adjustment);
	--m_numAllocations;
}