#ifndef SIMPLE_MEMORY_MANAGER_BASE_ALLOCATOR_GUARD
#define SIMPLE_MEMORY_MANAGER_BASE_ALLOCATOR_GUARD

#include <cstddef>
#include <cstdint>
#include "Util.h"

namespace SimpleMemoryAllocator {

	/**
	* The base custom allocator class containing all the common interface methods and members. All allocators inherit from this class.
	*/
	class IAllocator {
	private:

	protected:
		void*	m_start;			/// pointer to the beginning of the allocated memory
		size_t	m_size;				/// size of the allocated memory in bytes
		size_t	m_usedMemory;		/// amount of used memory in bytes
		size_t	m_numAllocations;	/// allocation counter, increments with allocations and decrements with deallocations

		// main logic methods, these are to be implemented in specific allocators
		virtual void* __allocate(size_t size, uint8_t alignment = 0) = 0;
		virtual void __deallocate(void* ptr) = 0;
	public:
		IAllocator(void* start, size_t size) {
			m_start = start;
			m_size = size;
			m_usedMemory = 0;
			m_numAllocations = 0;
		}

		/**
		* Default destructor, able to detect "memory leaks" with an assert.
		*/
		virtual ~IAllocator() {
			throw_assert(m_numAllocations == 0 || m_usedMemory == 0, "all memory should be deallocated");

			m_start = nullptr;
			m_size = 0;
		}


		// interface getter methods
		void* getStart() const { return m_start; }
		size_t getSize() const { return m_size; }
		size_t getUsedMemory() const { return m_usedMemory; }
		size_t getNumAllocations() const { return m_numAllocations; }


		// allocator interface methods

		/**
		* Allocates a single object of specified class.
		*/
		template <class T> T* allocate() {
			return new (__allocate(sizeof(T), alignof(T))) T;
		}


		/**
		* Allocates a single object of specified class with copy constructor.
		* @param t an instance of class T to be copied to the newly allocated one
		*/
		template <class T> T* allocate(const T& t) {
			return new (__allocate(sizeof(T), alignof(T))) T(t);
		}

		/**
		* Deallocates a single object specified by a pointer.
		* @param a pointer to a previously allocated object
		*/
		template <class T> void deallocate(T& object) {
			object.~T();
			__deallocate(&object);
		}

		/**
		* Allocates an array of objects of specified class.
		* @param length size of the array
		*/
		template <class T> T* allocateArray(size_t length) {
			throw_assert(length > 0, "allocated array length must be larger than 0");

			// calculate how many T-sized chucks we need to allocate to store the array length
			uint8_t headerSize = sizeof(size_t) / sizeof(T);
			if (sizeof(size_t) % sizeof(T) > 0) 
				headerSize += 1;

			// allocate extra memory before the array to store its size
			T* ptr = ((T*) __allocate(sizeof(T) * (length + headerSize), alignof(T))) + headerSize;
			*( ((size_t*)ptr) - 1 ) = length;
			
			// initialize all array elements
			for (size_t i = 0; i < length; ++i)
				new (&ptr) T;

			return ptr;
		}

		/**
		* Deallocates an array of objects specified by a pointer.
		* @param array a pointer to a previously allocated array
		*/
		template <class T> void deallocateArray(T* array) {
			throw_assert(array != nullptr, "deallocated array pointer must not be null");

			// calculate header size
			uint8_t headerSize = sizeof(size_t) / sizeof(T);
			if (sizeof(size_t) % sizeof(T) > 0)
				headerSize += 1;

			// get array length
			size_t length = *( ((size_t*)array) - 1 );

			// destroy all array elements
			for (size_t i = 0; i < length; ++i)
				array[i].~T();

			// deallocate the memory
			__deallocate(array - headerSize);
		}
	};
}

#endif