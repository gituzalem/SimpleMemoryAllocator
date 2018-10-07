#ifndef SIMPLE_MEMORY_MANAGER_STACK_ALLOCATOR_GUARD
#define SIMPLE_MEMORY_MANAGER_STACK_ALLOCATOR_GUARD

#include "IAllocator.h"

namespace SimpleMemoryAllocator {

	/**
	* An allocator working like a stack. Therefore, all allocated memory must be deallocated
	* in a LIFO fashion: if you allocated A first and then B, you must first deallocate B before 
	* deallocating A.
	*/
	class StackAllocator : public IAllocator {
	private:
		void* m_top;            /// pointer to the element on the top of the stack
		void* m_previousTop;    /// pointer to the element below the element on the top of the stack

		void* __allocate(size_t, uint8_t);
		void __deallocate(void*);
	public:
		/**
		* @brief A regular constructor that allocates a specified number of bytes from system.
		* 
		* @param 	memory_size 	size of the memory used by allocator in bytes
		*/
		StackAllocator(size_t memory_size);

		/**
		* @brief A regular constructor that uses a specified number of bytes from a given memory block.
		* 
		* @param	memory_ptr  	pointer to an already allocated system memory to be used by the allocator
		* @param 	memory_size 	size of the memory used by allocator in bytes	 	
		*/
		StackAllocator(void* memory_ptr, size_t memory_size);

		virtual ~StackAllocator();
	};

	struct StackAllocationHeader {
		uint8_t adjustment;
	};

}

#endif