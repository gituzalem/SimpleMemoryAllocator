#include <MemUtils.h>

using namespace SimpleMemoryAllocator;

uintptr_t MemoryUtils::get_next_aligned_address(void* address, uint8_t alignment) {
	return (((uintptr_t)(address) + alignment) & ~(alignment - 1));
}

uint8_t MemoryUtils::get_next_address_adjustment(void* address, uint8_t alignment) {
	// adjustment <= alignment
	auto adjustment = (alignment - ((uintptr_t)(address) & (alignment - 1)));

	if (adjustment == alignment)
		return 0;

	return adjustment;
}

uint8_t MemoryUtils::get_next_address_adjustment_with_header(void* address, uint8_t alignment, uint8_t header_size) {
	uint8_t adjustment = MemoryUtils::get_next_address_adjustment(address, alignment);
	uint8_t needed_space = header_size;

	if (adjustment < needed_space) {
		needed_space -= adjustment;

		// increase adjustment to fit header 
		adjustment += alignment * (needed_space / alignment);

		if (needed_space % alignment > 0) adjustment += alignment;
	}

	return adjustment;
}

void* MemoryUtils::add_to_pointer(void* address, size_t add) {
	return (static_cast<char*>(address) + add);
}
