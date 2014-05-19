
//          Copyright Seth Hendrick 2014
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "FilenameCheckerException.h"

namespace Common {

FilenameCheckerException::FilenameCheckerException(const std::string &what) :
    m_what(what)
{
}

FilenameCheckerException::~FilenameCheckerException() {
}

const char* FilenameCheckerException::what() const throw() {
    return m_what.c_str();
}

}
