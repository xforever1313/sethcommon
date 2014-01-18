#include "FilenameCheckerException.h"

namespace Common{
FilenameCheckerException::FilenameCheckerException(std::string what) :
    m_what(what)
{
}

FilenameCheckerException::~FilenameCheckerException(){
}

const char* FilenameCheckerException::what() const throw(){
    return m_what.c_str();
}

}
