
//          Copyright Seth Hendrick 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cstdio>
#include <string>

#include "cstdioWrapper.h"

namespace OS{

struct FILE_t{
    FILE *file;
};

const int cstdioWrapper::END_OF_FILE = EOF;

cstdioWrapper::cstdioWrapper(){
}

cstdioWrapper::~cstdioWrapper(){
}

int cstdioWrapper::remove(const std::string &filename){
    return std::remove(filename.c_str());
}

int cstdioWrapper::rename(const std::string &oldname, const std::string &newname){
    return std::rename(oldname.c_str(), newname.c_str());
}

FILE_t *cstdioWrapper::fopen(const std::string &filename, const std::string &mode){
    FILE_t *file = NULL;
    FILE *stream = ::fopen(filename.c_str(), mode.c_str());
    if (stream != NULL){
        file = new FILE_t;
        file->file = stream;
    }
    return file;
}

int cstdioWrapper::fgetc(FILE_t *stream){
    return ::fgetc(stream->file);
}

int cstdioWrapper::fputc(int character, FILE_t *stream){
    return ::fputc(character, stream->file);
}

int cstdioWrapper::ferror(FILE_t *stream){
    return ::ferror(stream->file);
}

int cstdioWrapper::feof(FILE_t *stream){
    return ::feof(stream->file);
}

int cstdioWrapper::fclose(FILE_t *stream){
    int rc = ::fclose(stream->file);
    delete stream;
    return rc;
}

}
