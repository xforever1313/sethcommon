
//          Copyright Seth Hendrick 2014
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef FILENAMECHECKER_H
#define FILENAMECHECKER_H

#include <string>

#include "FilenameCheckerException.h"

namespace Common{

/**
* \class FilenameChecker
* \brief Replaces file name symbols with symbols that are okay for a file system
* \note Symbols that get replaced: /\:*&?"'<>|^+ (space)
* \author Seth Hendrick
*/
class FilenameChecker{
    public:
        virtual ~FilenameChecker();

        static const unsigned int MAX_FILE_SIZE = 60;

        static const std::string ESCAPE_CHARACTER;

        ///forbidden characters
        static const std::string BACK_SLASH;
        static const std::string BACK_SLASH_REPLACE;

        static const std::string FRONT_SLASH;
        static const std::string FRONT_SLASH_REPLACE;

        static const std::string COLON;
        static const std::string COLON_REPLACE;

        static const std::string ASTERIC;
        static const std::string ASTERIC_REPLACE;

        static const std::string AMP;
        static const std::string AMP_REPLACE;

        static const std::string QUESTION;
        static const std::string QUESTION_REPLACE;

        static const std::string QUOTE;
        static const std::string QUOTE_REPLACE;

        static const std::string APOS;
        static const std::string APOS_REPLACE;

        static const std::string LT;
        static const std::string LT_REPLACE;

        static const std::string GT;
        static const std::string GT_REPLACE;

        static const std::string PIPE;
        static const std::string PIPE_REPLACE;

        static const std::string CARROT;
        static const std::string CARROT_REPLACE;

        static const std::string PLUS;
        static const std::string PLUS_REPLACE;

        static const std::string SPACE;
        static const std::string SPACE_REPLACE;

        static const std::string PERIOD;
        static const std::string PERIOD_REPLACE;

        /**
        * \throw FileCheckerException when fileName is greater than MAX_FILE_SIZE
        */
        static std::string checkFilename(std::string fileName);

    private:
        /**\brief default constructor, all methods are static,
        * therefore private constructor
        */
        FilenameChecker();

        static void toLowerCase(std::string &s);
};

}
#endif // FILENAMECHECKER_H
