#ifndef SIMPLE_MEMORY_MANAGER_UTILS_GUARD
#define SIMPLE_MEMORY_MANAGER_UTILS_GUARD

#include <cstddef>
#include <cstdint>
#include <memory>
#include <exception>
#include <string>
#include <sstream>

namespace SimpleMemoryAllocator {
	namespace MemoryUtils {
		uintptr_t getNextAlignedAddress(void* address, uint8_t alignment);
		uint8_t getNextAddressAdjustment(void* address, uint8_t alignment);
		uint8_t getNextAddressAdjustmentWithHeader(void* address, uint8_t alignment, uint8_t headerSize);
	} // namespace MemoryUtils


	/**
	* A custom exception accompanied with a C assert-style macro.
	* Inspired by https://www.softwariness.com/articles/assertions-in-cpp/
	*/
	class AssertException : public std::exception {
	private:
		const char*		m_expression;
		const char*		m_file;
		int				m_line;
		std::string		m_message;
		std::string		m_report;
	public:
		AssertException(const char* expression, const char* file, int line, std::string message) 
		: m_expression(expression)
		, m_file(file)
		, m_line(line)
		, m_message(message) {

			std::ostringstream ostream;

			if (!message.empty()) {
				ostream << message << ": ";
			}

			ostream << "Assertion '" << expression << "' failed in file '" << m_file << "' line " << m_line;

			m_report = ostream.str();
		}

		~AssertException() throw() {  }

		virtual const char* what() const throw() {
			return m_report.c_str();
		}
	 
		const char* getExpression() const throw() {
			return m_expression;
		}
	 
		const char* getFile() const throw() {
			return m_file;
		}
	 
		int getLine() const throw() {
			return m_line;
		}
	 
		const char* getMessage() const throw() {
			return m_message.c_str();
		}
	};

	#define throw_assert(EXPRESSION, MESSAGE) if(!(EXPRESSION)) { throw AssertException(#EXPRESSION, __FILE__, __LINE__, MESSAGE); }

} // namespace SimpleMemoryAllocator


#endif