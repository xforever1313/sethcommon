
//          Copyright Seth Hendrick 2014
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef FILELOGGER_H
#define FILELOGGER_H

#include <fstream>
#include <memory>
#include <stdexcept>
#include <string>

#include "io/LoggerBase.h"

namespace Common {
namespace IO {

/**
 * \brief This struct is a work around so an ofstream will never go out of scope.
 */
struct FileLoggerImpl {
        FileLoggerImpl() {

        }

        virtual ~FileLoggerImpl() {
        }

    protected:
        std::ofstream m_fs;
};

class FileLogger : public FileLoggerImpl, public LoggerBase {
    public:
        /**
         * \brief Creates, opens, and returns an instance of FileLogger
         * \throw std::runtime_error if an error occurs when opening.
         * \note This is a shared_ptr so move constructors don't need to be dealt with.
         */
        static std::shared_ptr<FileLogger> createFileLogger(const std::string &fileLocation);

        /**
         * \brief Closes (if its not already) and deallocates the FileLogger
         */
        ~FileLogger();

        /**
         * \brief Closes the stream.  Any other calls to Write or Writeline will have no effect.
         */
        void close();

    private:
        FileLogger();

        FileLogger (const FileLogger&) = delete;
        FileLogger &operator=(const FileLogger&) = delete;

        bool m_isOpen;
};

}
}

#endif // FILELOGGER_H
