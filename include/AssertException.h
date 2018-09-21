#include <exception>
#include <string>
#include <sstream>

/**
* A custom exception accompanied with a C assert-style macro.
* Inspired by https://www.softwariness.com/articles/assertions-in-cpp/
*/
namespace SimpleMemoryAllocator {

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

    ~AssertException() noexcept {  }

    virtual const char* what() const noexcept {
        return m_report.c_str();
    }

    const char* get_expression() const noexcept {
        return m_expression;
    }

    const char* get_file() const noexcept {
        return m_file;
    }

    int get_line() const noexcept {
        return m_line;
    }

    const char* get_message() const noexcept {
        return m_message.c_str();
    }
};

#define throw_assert(EXPRESSION, MESSAGE) if(!(EXPRESSION)) { throw AssertException(#EXPRESSION, __FILE__, __LINE__, MESSAGE); }

}