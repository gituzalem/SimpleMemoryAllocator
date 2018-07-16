#ifndef SIMPLE_MEMORY_MANAGER_LINEAR_ALLOCATOR_GUARD
#define SIMPLE_MEMORY_MANAGER_LINEAR_ALLOCATOR_GUARD

#include "IAllocator.h"

namespace SimpleMemoryAllocator {

	/**
	* An allocator that allocates memory in a linear fashion. Being the simplest, and therefore
	* the fastest allocator. It cannot deallocate its memory randomly, but rather has to clear it completely.
	*/
	class LinearAllocator : public IAllocator {
	private:
		void* m_firstFree;	/// the nearest free address

		void* __allocate(size_t, uint8_t);
		void __deallocate(void*);
	public:
		/**
		* @brief A default constructor that allocates a specified number of bytes from system.
		*
		* @param	memorySize 	size of the memory used by the allocator in bytes
		*/
		LinearAllocator(size_t memorySize);

		/**
		* @brief A default constructor that allocates a specified number of bytes from given memory block.
		*
		* @param	memoryPtr	pointer to an already allocated system memory to be used by the allocator
		* @param	memorySize 	size of the memory used by the allocator in bytes
		*/
		LinearAllocator(void* memoryPtr, size_t memorySize);

		~LinearAllocator();

		/**
		* @brief Clears the entire allocator memory. Replaces the __deallocate() function, which cannot be used in this case.
		*/
		void clear();
	};

}

#endif