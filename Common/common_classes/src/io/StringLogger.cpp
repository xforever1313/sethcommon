
//          Copyright Seth Hendrick 2014
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

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
