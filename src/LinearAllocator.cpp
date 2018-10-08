#include <LinearAllocator.h>

using namespace SimpleMemoryAllocator;

LinearAllocator::LinearAllocator(size_t memory_size) : BaseAllocator(nullptr, memory_size), m_firstFree(m_start) { }

LinearAllocator::LinearAllocator(void* memory_ptr, size_t memory_size) : BaseAllocator(memory_ptr, memory_size), m_firstFree(m_start) { }

LinearAllocator::~LinearAllocator() {
	m_firstFree = nullptr;
}

void* LinearAllocator::__allocate(size_t size, uint8_t alignment) {
	throw_assert(size > 0, "allocated size must be larger than 0");

	uint8_t adjustment = MemoryUtils::get_next_address_adjustment(m_firstFree, alignment);

	// don't allocate if we need to allocate more than we have free
	if (m_used_memory + size + adjustment > m_size) return nullptr;

	void* alignedAddress = MemoryUtils::add_to_pointer(m_firstFree, adjustment);
	m_firstFree = MemoryUtils::add_to_pointer(alignedAddress, size);
	m_used_memory += size + adjustment;
	++m_num_allocations;

	return alignedAddress;
}

void LinearAllocator::__deallocate(void* ptr) {
	throw_assert(false, "method deallocate() is not usable in a linear allocator, use method clear() instead");
}

void LinearAllocator::clear() {
	m_num_allocations = 0;
	m_used_memory = 0;
	m_firstFree = m_start;
}