#include "StackAllocator.h"

using namespace SimpleMemoryAllocator;

StackAllocator::StackAllocator(size_t memory_size) : StackAllocator(nullptr, memory_size) { }

StackAllocator::StackAllocator(void* memory_ptr, size_t memory_size) : IAllocator(memory_ptr, memory_size), m_top(m_start), m_previousTop(nullptr) {  }

StackAllocator::~StackAllocator() {
	m_top = nullptr;
	m_previousTop = nullptr;
}

void* StackAllocator::__allocate(size_t size, uint8_t alignment) {
	throw_assert(size > 0, "allocated size must be larger than 0");

	uint8_t adjustment = MemoryUtils::get_next_address_adjustment_with_header(m_top, alignment, sizeof(StackAllocationHeader));

	// don't allocate if we need to allocate more than we have free
	if (m_used_memory + size + adjustment > m_size) return nullptr;

	void* alignedAddress = MemoryUtils::add_to_pointer(m_top, adjustment);

	// store allocation header before the actual stored data
	StackAllocationHeader* header = (StackAllocationHeader*)MemoryUtils::add_to_pointer(alignedAddress, -sizeof(StackAllocationHeader));
	header->adjustment = adjustment;

	m_previousTop = m_top;
	m_top = MemoryUtils::add_to_pointer(alignedAddress, size);
	m_used_memory += size + adjustment;
	++m_num_allocations;

	return (void*)alignedAddress;
}

void StackAllocator::__deallocate(void* ptr) {
	throw_assert(ptr != nullptr, "deallocated pointer must not be null");

	StackAllocationHeader* header = (StackAllocationHeader*)MemoryUtils::add_to_pointer(ptr, -sizeof(StackAllocationHeader));
	m_used_memory -= ((unsigned*)m_top - (unsigned*)ptr + header->adjustment);
	m_top = MemoryUtils::add_to_pointer(ptr, header->adjustment);
	--m_num_allocations;
}