#ifndef SIMPLE_MEMORY_MANAGER_BASE_ALLOCATOR_GUARD
#define SIMPLE_MEMORY_MANAGER_BASE_ALLOCATOR_GUARD

#include <cstddef>
#include <cstdint>
#include <mutex>
#include "AssertException.h"
#include "MemUtils.h"

namespace SimpleMemoryAllocator {

	/**
	* The base custom allocator class containing all the common interface methods and members. All allocators inherit from this class.
	*/
	class IAllocator {
	private:
		bool	    m_handling_memory_internally = false;  /// a boolean flag to indicate the allocator is handling the system memory allocation
		std::mutex  m_allocator_mutex;

	protected:
		void*       m_start;                              /// pointer to the beginning of the allocated memory
		size_t      m_size;                               /// size of the allocated memory in bytes
		size_t      m_used_memory;                        /// amount of used memory in bytes
		size_t      m_num_allocations;                    /// allocation counter, increments with allocations and decrements with deallocations

		IAllocator(const IAllocator&) = delete;	          // disable copy-constructor

		////////////////////////////////////////////////////////////////////////////
		// main logic methods, these are to be implemented in specific allocators //
		//////////////////////////////////////////////////////////////////////////////////////////////
		/**
		* @brief An internal method handling new variable allocation. Specific for each allocator type.
		*
		* @param	size        size of allocated variable in bytes
		* @param	alignment   memory alignment of the variable type
		*
		* @return a pointer to a newly allocated variable
		*/
		virtual void* __allocate(size_t size, uint8_t alignment = 0) = 0;

		/**
		* @brief An internal method handling variable deallocation. Specific for each allocator type.
		*
		* @param	ptr         pointer to a memory previously allocated by __allocate()
		*/
		virtual void __deallocate(void* ptr) = 0;

	public:
		/**
		* @brief Standard constructor, initializes the basic necessary allocator data. 
		*
		* If start is nullptr, it uses native C++ ::operator new to allocate needed system memory.
		* 
		* @param	start       a pointer to the beginning of the allocated memory space
		* @param	size        size of the allocated memory space in bytes
		*/
		IAllocator(void* start, size_t size) {
			m_start = (start != nullptr ? start : allocate_memory_native(size));
			m_size = size;
			m_used_memory = 0;
			m_num_allocations = 0;
		}

		/**
		* @brief Default destructor, able to detect nondeallocated memory and possible deallocation faults
		*/
		virtual ~IAllocator() {
			throw_assert(m_num_allocations == 0 || m_used_memory == 0, "all memory should be deallocated");

			deallocate_memory_native();

			m_start = nullptr;
			m_size = 0;
		}

	private:
		///////////////////////////////////////////////////
		// native memory allocation/deallocation methods //
		//////////////////////////////////////////////////////////////////////////////////////////////

		/**
		* @brief Allocates system memory using ::operator new.
		*
		* @param	sizeBytes	allocated memory size in bytes
		*
		* @return a void pointer to the newly allocated memory
		*/
		inline void* allocate_memory_native(size_t sizeBytes) {
			m_handling_memory_internally = true;
			return ::operator new(sizeBytes);
		}
		
		/**
		* @brief Deallocates the system memory allocated by allocateMemoryNative()
		*/
		inline void deallocate_memory_native() {
			if (m_handling_memory_internally)
				::operator delete(m_start);
		}

	public:
		//////////////////////////////
		// interface getter methods //
		//////////////////////////////////////////////////////////////////////////////////////////////
		/// initial memory position getter
		void* get_start() const noexcept { return m_start; }
		/// allocator size getter
		size_t get_size() const noexcept { return m_size; }
		/// memory used by allocator (in bytes) getter
		size_t get_used_memory() const noexcept { return m_used_memory; }
		/// number of active allocations getter
		size_t get_num_allocations() const noexcept { return m_num_allocations; }


		/////////////////////////////////
		// allocator interface methods //
		//////////////////////////////////////////////////////////////////////////////////////////////

		/**
		* @brief Allocates a single object of specified class.
		*
		* @param	T	template type of the new variable
		*
		* @return a pointer to the newly allocated class instance
		*/
		template <class T> T* allocate() {
			return new (__allocate(sizeof(T), alignof(T))) T;
		}

		/**
		* @brief Allocates a single object of specified class in a thread-safe manner.
		*
		* @param	T	template type of the new variable
		*
		* @return a pointer to the newly allocated class instance
		*/
		template <class T> T* allocate_thread_safe() {
			std::lock_guard<std::mutex> lock(m_allocator_mutex);
			return allocate<T>();
		}

		/**
		* @brief Allocates a single object of specified class with copy constructor.
		* 
		* @param	T	template type of the new variable
		* @param	t	an instance of class T to be copied to the newly allocated one
		*
		* @return a pointer to the newly allocated class instance
		*/
		template <class T> T* allocate(const T& t) {
			return new (__allocate(sizeof(T), alignof(T))) T(t);
		}

		/**
		* @brief Allocates a single object of specified class with copy constructor in a thread-safe manner.
		* 
		* @param	T	template type of the new variable
		* @param	t	an instance of class T to be copied to the newly allocated one
		*
		* @return a pointer to the newly allocated class instance
		*/
		template <class T> T* allocate_thread_safe(const T& t) {
			std::lock_guard<std::mutex> lock(m_allocator_mutex);
			return allocate<T>(t);
		}

		/**
		* @brief Deallocates a single object specified by a pointer.
		* 
		* @param	T	template type of the deleted variable
		* @param	object	pointer to a previously allocated object
		*/
		template <class T> void deallocate(T& object) {
			object.~T();
			__deallocate(&object);
		}

		/**
		* @brief Deallocates a single object specified by a pointer in a thread-safe manner.
		* 
		* @param	T	template type of the deleted variable
		* @param	object	pointer to a previously allocated object
		*/
		template <class T> void deallocate_thread_safe(T& object) {
			std::lock_guard<std::mutex> lock(m_allocator_mutex);
			deallocate(object);
		}

		/**
		* @brief Allocates an array of objects of specified class.
		* 
		* @param	T	template type of the new array
		* @param	length	size of the array
		*
		* @return a pointer to the newly allocated class instance array
		*/
			template <class T> T* allocate_array(size_t length) {
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
		* @brief Allocates an array of objects of specified class in a thread-safe manner.
		* 
		* @param	T	template type of the new array
		* @param	length	size of the array
		*
		* @return a pointer to the newly allocated class instance array
		*/
		template <class T> T* allocate_array_thread_safe(size_t length) {
			std::lock_guard<std::mutex> lock(m_allocator_mutex);
				return allocate_array<T>(length);
		}

		/**
		* @brief Deallocates an array of objects specified by a pointer.
		* 
		* @param	T	template type of the deleted array
		* @param	array	a pointer to a previously allocated array
		*/
		template <class T> void deallocate_array(T* array) {
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

		/**
		* @brief Deallocates an array of objects specified by a pointer in a thread-safe manner.
		* 
		* @param	T	template type of the deleted array
		* @param	array	a pointer to a previously allocated array
		*/
		template <class T> void deallocate_array_thread_safe(T* array) {
			std::lock_guard<std::mutex> lock(m_allocator_mutex);
			deallocate_array(array);
		}
	};
}

#endif