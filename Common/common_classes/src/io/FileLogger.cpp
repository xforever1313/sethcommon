
//          Copyright Seth Hendrick 2014
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <fstream>
#include <memory>
#include <stdexcept>
#include <string>

#include "io/FileLogger.h"
#include "io/LoggerBase.h"

namespace Common {
namespace IO {

std::shared_ptr<FileLogger> FileLogger::createFileLogger(const std::string &fileLocation) {
    std::shared_ptr<FileLogger> fl (new FileLogger);
    fl->m_fs.open(fileLocation);
    if (!fl->m_fs.is_open()) {
        throw std::runtime_error ("Could not open " + fileLocation + " for writing.");
    }
    fl->m_isOpen = true;

    return fl;
}

FileLogger::FileLogger() :
    FileLoggerImpl(),
    LoggerBase(m_fs),
    m_isOpen(false)
{
}

FileLogger::~FileLogger() {
    close();
}

void FileLogger::close() {
    if (m_isOpen) {
        m_fs.close();
        m_isOpen = false;
    }
}

}
}
