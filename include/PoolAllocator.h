#ifndef SIMPLE_MEMORY_MANAGER_POOL_ALLOCATOR_GUARD
#define SIMPLE_MEMORY_MANAGER_POOL_ALLOCATOR_GUARD

#include <BaseAllocator.h>

namespace SimpleMemoryAllocator {

	/**
	* An allocator working as a "pool" of equally sized objects. Since the size of the
	* objects is constant and known in advance, this reduces the memory and processing overhead.
	* Also negates the bad effect of memory fragmentation, sice every time a block is freed, it is 
	* guaranteed to be able to hold another one in its place.
	*/
	class PoolAllocator : public BaseAllocator {
	private:
		void** m_freeList;          /// a linked list of all currectly unused pool elements
		size_t m_objectSize;        /// size of the stored type
		uint8_t m_objectAlignment;  /// memory alignment of the stored type

		void* __allocate(size_t, uint8_t);
		void __deallocate(void*);
	public:
		/**
		* @brief Simplified constructor that creates pool for a specified type with a specified size.
		
		* This constructor calls the PoolAllocator(size_t memory_size, size_taobject_size, uint8_t object_slignment) constructor with 
		* the appropriate arguments derived from the type and pool size.
		*
		* @param	T	template type of the pool
		* @param	pool_size	number of T-sized elements that the pool should fit
		*/
		template <class T> PoolAllocator(size_t pool_size);

		/**
		* @brief Simplified constructor that creates pool for a specified type with a specified size, starting at a specified point in memory.
		
		* This constructor calls the PoolAllocator(void* memory_ptr, size_t mamory_size, size_t object_size, uint8_t object_slignment) constructor with 
		* the appropriate arguments derived from the type and pool size.
		*
		* @param	T	template type of the pool
		* @param	memory_ptr	pointer to an already allocated system memory to be used by the allocator
		* @param	pool_size	number of T-sized elements that the pool should fit
		*/
		template <class T> PoolAllocator(void* memory_ptr, size_t pool_size);


		/**
		* @brief A regular constructor that creates a pool allocator with memory size, object size and object alignment explicitely stated.
		* 
		* @param	memory_size	size of the memory used by the allocator in bytes
		* @param	object_size	size of a single pool element in bytes
		* @param	object_slignment		memory alignment of the stored object type
		*/
		PoolAllocator(size_t memory_size, size_t object_size, uint8_t object_alignment);

		/**
		* @brief A regular constructor that creates a pool allocator with memory size, object size and object alignment explicitely stated.
		*
		* @param	memory_ptr	pointer to an already allocated system memory to be used by the allocator
		* @param	memory_size	size of the memory used by the allocator in bytes
		* @param	object_size	size of a single pool element in bytes
		* @param	object_slignment		memory alignment of the stored object type
		*/
		PoolAllocator(void* memory_ptr, size_t memory_size, size_t object_siza, uint8_t object_slignment);

		virtual ~PoolAllocator();
	};

}

#endif