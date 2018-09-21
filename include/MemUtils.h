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
		uintptr_t get_next_aligned_address(void* address, uint8_t alignment);

		/**
		* @brief Computes the adjustment needed for the nearest aligned address.
		*
		* @param	address		original address
		* @param	alignment	type memory alignment size
		*
		* @return the adjustment required to obtain a nearest aligned address in bytes
		*/
		uint8_t get_next_address_adjustment(void* address, uint8_t alignment);

		/**
		* @brief Computes the adjustment needed for the nearest aligned address when using an allocation header.
		*
		* @param	address		original address
		* @param	alignment	type memory alignment size
		* @param	header_size	size of the allocation header in bytes
		*
		* @return the adjustment required to obtain the nearest aligned address in bytes
		*/
		uint8_t get_next_address_adjustment_with_header(void* address, uint8_t alignment, uint8_t header_size);

		/**
		* @brief Performs an arithmetic addition of a number to a memory address.
		* 
		* @param	address		original address
		* @param	add		the added number
		* 
		* @return the new address obtained by addition
		*/
		void* add_to_pointer(void* address, size_t add);
	} // namespace MemoryUtils



} // namespace SimpleMemoryAllocator


#endif