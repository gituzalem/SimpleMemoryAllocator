#ifndef SIMPLE_MEMORY_MANAGER_BASE_ALLOCATOR_GUARD
#define SIMPLE_MEMORY_MANAGER_BASE_ALLOCATOR_GUARD

#include <cstddef>
#include <cstdint>

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
			if (m_numAllocations != 0 || m_usedMemory != 0) {
				// TODO throw exception or something
			}
			m_start = nullptr;
			m_size = 0;
		}

		// main interface methods, these are to be implemented in specific allocators
		virtual void* allocate(size_t size, uint8_t alignment = 0) = 0;
		virtual void deallocate(void* ptr) = 0;

		// interface getter methods
		void* getStart() const { return m_start; }
		size_t getSize() const { return m_size; }
		size_t getUsedMemory() const { return m_usedMemory; }
		size_t getNumAllocations() const { return m_numAllocations; }
	};

	/**
	* @todo documentation
	*/
	template <class T> T* allocateNew(IAllocator& allocator) {
		return new (allocator.allocate(sizeof(T), alignof(T))) T;
	}

	/**
	* @todo documentation
	*/
	template <class T> T* allocateNew(IAllocator& allocator, const T& t) {
		return new (allocator.allocate(sizeof(T), alignof(T))) T(t);
	}

	/**
	* @todo documentation
	*/
	template <class T> void deallocateDelete(IAllocator& allocator, T& object) {
		object.~T();
		allocator.deallocate(&object);
	}
}

#endif