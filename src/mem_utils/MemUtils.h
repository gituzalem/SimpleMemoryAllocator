#ifndef SIMPLE_MEMORY_MANAGER_UTILS_GUARD
#define SIMPLE_MEMORY_MANAGER_UTILS_GUARD

#include <cstddef>
#include <cstdint>
#include <memory>

namespace SimpleMemoryAllocator {
		/**
		* @brief A namespace encompassing all auxillary functions concerning either memory alignment or any other kind of memory address manipulation.
		*/
	namespace MemoryUtils {

		/**
		* @brief Computes the nearest aligned address.
		*
		* @param	address		original address
		* @param	alignment	type memory alignment size
		*
		* @return the nearest aligned memory address
		*/
		uintptr_t getNextAlignedAddress(void* address, uint8_t alignment);

		/**
		* @brief Computes the adjustment needed for the nearest aligned address.
		*
		* @param	address		original address
		* @param	alignment	type memory alignment size
		*
		* @return the adjustment required to obtain a nearest aligned address in bytes
		*/
		uint8_t getNextAddressAdjustment(void* address, uint8_t alignment);

		/**
		* @brief Computes the adjustment needed for the nearest aligned address when using an allocation header.
		*
		* @param	address		original address
		* @param	alignment	type memory alignment size
		* @param	headerSize	size of the allocation header in bytes
		*
		* @return the adjustment required to obtain the nearest aligned address in bytes
		*/
		uint8_t getNextAddressAdjustmentWithHeader(void* address, uint8_t alignment, uint8_t headerSize);

		/**
		* @brief Performs an arithmetic addition of a number to a memory address.
		* 
		* @param	address		original address
		* @param	add		the added number
		* 
		* @return the new address obtained by addition
		*/
		void* addToPointer(void* address, size_t add);
	} // namespace MemoryUtils



} // namespace SimpleMemoryAllocator


#endif