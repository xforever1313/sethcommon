
//          Copyright Seth Hendrick 2014
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef LOGGER_BASE_H_
#define LOGGER_BASE_H_

#include <mutex>
#include <ostream>
#include <string>

namespace Common {
namespace IO {

/**
* \brief A generic, threadsafe logger.
* \details The problem with streams such as std::cout is that they are
*          not threadsafe.  When someone does std::cout << 3 << "Hi" << std::endl;
*          each '<<' is a separate function call.  If another thread has the same
*          call, cout may print "33HiHi", if one thread stops after it prints 3,
*          and the other takes over.
*
*          This class surrounds all ostream calls with a mutex, thus ensuring that
*          a message does not get interrupted by another thread.
*
*          Unlike other languages, such as C# and Java, you can not concatinate
*          primatatives (int, char) with strings directly.  So, you need to use
*          something like std::to_string(int) to convert an int to a string
*          before concatinating it.
*
*          LoggerBase::write("4 - 3 = " + std::to_string(1));
*
*          However, std::to_string has limitations.  For example, you can not set
*          the precision for floats, nor can you use std::boolalpha for boolean values.
*          If you want precision, or something else, you need to use a stringstream
*          before calling write.
*
*          std::stringstream ss;
*
*          ss << std::boolalpha << true;
*
*          LoggerBase::write(ss.str());
*/

class LoggerBase {
    public:
        /**
         * \warning Ensure that the passed in ostream does not
         *          get deleted or go out of scrope while its corresponding
         *          LoggerBase object exists.
         */
        LoggerBase (std::ostream &os);

        virtual ~LoggerBase();

        /**
         * \brief Writes the the log.  DOES NOT add a new line.
         */
        void write(const std::string &s);

        /**
         * \brief Writes to the log, and adds a new line at the end of the message.
         */
        void writeLine(const std::string &s);

        /**
         * \brief Writes to a log with the data in front of the message.
         */
        void writeLineWithTimeStamp(const std::string &s);

    protected:
        std::mutex m_mutex;

    private:
        LoggerBase() = delete;

        /**
         * \brief Uncopyable as both ostream and mutex can not be copied.
         */
        LoggerBase(const LoggerBase &) = delete;
        LoggerBase &operator=(const LoggerBase&) = delete;

        std::ostream &m_os;
};

}
}

#endif
