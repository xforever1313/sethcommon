#ifndef SCSVCONSTANTS_H_INCLUDED
#define SCSVCONSTANTS_H_INCLUDED
namespace Common{
#define SCSV_SEPARATOR ','
#define SCSV_AMP '&'

#define SCSV_ESCAPE_CHAR '&'
#define SCSV_ESCAPE_END ';'

#define SCSV_SEPARATOR_REPLACE "&comma;"
#define SCSV_AMP_REPLACE "&amp;"

#define SCSV_DEFAULT_STRING ""

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
