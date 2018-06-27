#ifndef SIMPLE_MEMORY_MANAGER_UTILS_GUARD
#define SIMPLE_MEMORY_MANAGER_UTILS_GUARD

#include <cstddef>
#include <cstdint>
#include <memory>

namespace SimpleMemoryManager {
	namespace MemoryUtils {
		uintptr_t getNextAlignedAddress(void* address, uint8_t alignment);
		uint8_t getNextAddressAdjustment(void* address, uint8_t alignment);
		uint8_t getNextAddressAdjustmentWithHeader(void* address, uint8_t alignment, uint8_t headerSize);
	} // namespace MemoryUtils
} // namespace SimpleMemoryManager

#endif