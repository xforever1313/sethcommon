#include <mutex>
#include <ostream>
#include <string>

#include "io/LoggerBase.h"

namespace Common {
namespace IO {

LoggerBase::LoggerBase(std::ostream &os) :
    m_os(os)
{
}

LoggerBase::~LoggerBase() {

}

void LoggerBase::write(const std::string &s) {
    m_mutex.lock();
    m_os << s;
    m_os.flush();
    m_mutex.unlock();
}

void LoggerBase::writeLine(const std::string &s) {
    m_mutex.lock();
    m_os << s << std::endl;
    m_mutex.unlock();
}

}
}
