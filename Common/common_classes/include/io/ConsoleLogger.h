
//          Copyright Seth Hendrick 2014
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef CONSOLE_LOGGER_H_
#define CONSOLE_LOGGER_H_

#include "io/LoggerBase.h"

namespace Common {
namespace IO {

/**
 * \brief ConsoleLogger only contains static instances of
 *        Loggerbase for writing to the console through stdout and stderr.
 *
 * \details The reason why this is a class, and not just a namespace is
 *          so stdout and stderr can be initalized privately in the .cpp folder,
 *          and people with access to the header can not change what they get
 *          set to.
 */
class ConsoleLogger {
    public:
        static LoggerBase out; ///< Writes to stdout
        static LoggerBase err; ///< Writes to stderr

    private:
        ConsoleLogger() = delete;
        ConsoleLogger(const ConsoleLogger&) = delete;
        ConsoleLogger(const ConsoleLogger&&) = delete;
        ConsoleLogger &operator=(const ConsoleLogger&) = delete;
        ConsoleLogger &operator=(const ConsoleLogger&&) = delete;
};

}
}

#endif
