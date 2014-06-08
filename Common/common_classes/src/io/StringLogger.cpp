#include <sstream>
#include <string>

#include "io/LoggerBase.h"
#include "io/StringLogger.h"

namespace Common {
namespace IO {

StringLogger::StringLogger() :
    StringLoggerImpl(),
    LoggerBase(m_ss)
{

}

StringLogger::~StringLogger(){
}

const std::string StringLogger::getString() const {
    return m_ss.str();
}

}
}
