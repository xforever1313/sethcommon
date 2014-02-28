
//          Copyright Seth Hendrick 2014
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef FILENAMECHECKEREXCEPTION_H
#define FILENAMECHECKEREXCEPTION_H

#include <exception>
#include <string>

namespace Common{
class FilenameCheckerException : public std::exception{
    public:
        FilenameCheckerException(std::string what);
        virtual ~FilenameCheckerException();
        const char* what() const throw();
    private:
        FilenameCheckerException();
        std::string m_what;
};

}
#endif // FILENAMECHECKEREXCEPTION_H
