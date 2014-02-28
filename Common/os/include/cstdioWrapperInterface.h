
//          Copyright Seth Hendrick 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef CSTDIOWRAPPERINTERFACE_H_INCLUDED
#define CSTDIOWRAPPERINTERFACE_H_INCLUDED

namespace SkyvoOS{

struct FILE_t;

class cstdioWrapperInterface{
    public:
        virtual ~cstdioWrapperInterface(){};
        virtual int remove(const std::string &filename) = 0;
        virtual int rename(const std::string &oldname, const std::string &newname) = 0;
        virtual FILE_t *fopen(const std::string &filename, const std::string &mode) = 0;
        virtual int fgetc(FILE_t *stream) = 0;
        virtual int fputc(int chararacter, FILE_t *stream) = 0;
        virtual int ferror(FILE_t *stream) = 0;
        virtual int feof(FILE_t *stream) = 0;
        virtual int fclose(FILE_t *stream) = 0;
};

}

#endif // CSTDIOWRAPPERINTERFACE_H_INCLUDED
