#include "../include/Util.h"

using namespace SimpleMemoryAllocator;

uintptr_t MemoryUtils::getNextAlignedAddress(void* address, uint8_t alignment) {
	return (((uintptr_t)(address) + alignment) & ~(alignment - 1));
}

uint8_t MemoryUtils::getNextAddressAdjustment(void* address, uint8_t alignment) {
	auto adjustment = (alignment - ((uintptr_t)(address) & (alignment - 1)));

	if (adjustment == alignment)
		return 0;

	return adjustment;
}

uint8_t MemoryUtils::getNextAddressAdjustmentWithHeader(void* address, uint8_t alignment, uint8_t headerSize) {
	uint8_t adjustment = MemoryUtils::getNextAddressAdjustment(address, alignment);
	uint8_t neededSpace = headerSize;

	if (adjustment < neededSpace) {
		neededSpace -= adjustment;

		// increase adjustment to fit header 
		adjustment += alignment * (neededSpace / alignment);

		if (neededSpace % alignment > 0) adjustment += alignment;
	}

	return adjustment;
}
