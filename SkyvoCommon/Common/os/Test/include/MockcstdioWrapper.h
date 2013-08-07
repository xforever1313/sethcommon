#ifndef MOCKCSTDIOWRAPPER_H_INCLUDED
#define MOCKCSTDIOWRAPPER_H_INCLUDED

#include <gmock/gmock.h>

#include "cstdioWrapperInterface.h"

namespace SkyvoOS{

struct FILE_t{
};

class MockcstdioWrapper : public cstdioWrapperInterface{
    public:
        ~MockcstdioWrapper(){};
        MOCK_METHOD1(remove, int(const std::string &filename));
        MOCK_METHOD2(rename, int(const std::string &oldname, const std::string &newname));
        MOCK_METHOD2(fopen, FILE_t *(const std::string &filename, const std::string &mode));
        MOCK_METHOD1(fgetc, int(FILE_t *stream));
        MOCK_METHOD2(fputc, int(int chararacter, FILE_t *stream));
        MOCK_METHOD1(ferror, int(FILE_t *stream));
        MOCK_METHOD1(feof, int(FILE_t *stream));
        MOCK_METHOD1(fclose, int(FILE_t *stream));
};

}

#endif // MOCKCSTDIOWRAPPER_H_INCLUDED
