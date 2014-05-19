
//          Copyright Seth Hendrick 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef SCSVCONSTANTS_H_INCLUDED
#define SCSVCONSTANTS_H_INCLUDED

#include <string>

namespace SCSV{

struct SCSVConstants {
    static const char SCSV_SEPARATOR = ',';
    static const char SCSV_AMP = '&';

    static const char SCSV_ESCAPE_CHAR = '&';
    static const char SCSV_ESCAPE_END = ';';

	static const std::string SCSV_SEPARATOR_REPLACE;
	static const std::string SCSV_AMP_REPLACE;

	static const std::string SCSV_DEFAULT_STRING;
};

enum SCSVLoadErrors{
    SCSV_OKAY = 0,

    SCSV_OPEN_ERR = 1,

    SCSV_CORRUPTED_FILE,

    SCSV_ROW_MISSING_NEW_LINE, ///<When the row is missing a new line character

    SCSV_INVALID_ESCAPE_CODE,  ///<When an escape code is invalid

    SCSV_MISSING_ESCAPE_CODE_END ///<Escape code is missing ';'

    #ifdef UNIT_TEST
    ,
    SCSV_DEFAULT ///<Only used to get full coverage
    #endif
};

}
#endif // SCSVCONSTANTS_H_INCLUDED
