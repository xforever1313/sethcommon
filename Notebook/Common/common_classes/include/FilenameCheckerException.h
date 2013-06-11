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
