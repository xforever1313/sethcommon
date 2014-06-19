
//          Copyright Seth Hendrick 2014
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef STRING_LOGGER_H_
#define STRING_LOGGER_H_

#include <sstream>
#include <string>

#include "io/LoggerBase.h"

namespace Common {
namespace IO {

/**
 * \brief This struct is a work around so a stringstream will never go out of scope.
 */
struct StringLoggerImpl {
        StringLoggerImpl() {

        }

        virtual ~StringLoggerImpl() {
        }

    protected:
        std::stringstream m_ss;
};

/**
 * \brief this logger logs to a string, which is obtainable by
 * this classes getString() method.
 */
class StringLogger : public StringLoggerImpl, public LoggerBase {
    public:
        StringLogger();
        ~StringLogger();

        const std::string getString() const;
};

}
}

#endif
