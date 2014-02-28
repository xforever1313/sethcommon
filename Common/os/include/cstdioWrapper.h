
//          Copyright Seth Hendrick 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef CSTDIOWRAPPER_H
#define CSTDIOWRAPPER_H

#include "cstdioWrapperInterface.h"
#include <string>

namespace SkyvoOS{

/**
* \class cstdioWrapper
* \brief this class only wraps the cstdio header, that way it can be mocked.
*/

class cstdioWrapper : public cstdioWrapperInterface{
    public:
        cstdioWrapper();
        virtual ~cstdioWrapper();
        int remove(const std::string &filename);
        int rename(const std::string &oldname, const std::string &newname);
        FILE_t *fopen(const std::string &filename, const std::string &mode);
        int fgetc(FILE_t *stream);
        int fputc(int chararacter, FILE_t *stream);
        int ferror(FILE_t *stream);
        int feof(FILE_t *stream);

        /**
        * \brief closes and DELETES the stream.  Do not reuse the passed in pointer.
        * \warning Even if this call fails, the stream is deleted, as cstdio disassociates the file with streams anyways.
        */
        int fclose(FILE_t *stream);

        static const int END_OF_FILE;
};

}

#endif // CSTDIOWRAPPER_H
