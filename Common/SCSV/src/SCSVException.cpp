
//          Copyright Seth Hendrick 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "SCSVException.h"

namespace SCSV{

SCSVException::SCSVException(const SCSVLoadErrors &e) :
    m_error(e),
    m_errorMessage(SCSVException::getErrorMessage(e))
{
}

SCSVException::~SCSVException(){
}

SCSVLoadErrors SCSVException::getError() const{
    return m_error;
}

const char *SCSVException::what() const throw(){
    return m_errorMessage.c_str();
}

std::string SCSVException::getErrorMessage(const SCSVLoadErrors &errorNum){
    std::string s = "";

    switch (errorNum){

        case SCSV_OKAY:
            s = "";
        break;

        case SCSV_OPEN_ERR:
            s = "Could not open CSV file";
        break;

        case SCSV_CORRUPTED_FILE:
            s = "The CSV file seems to have been corrupted";
        break;

        case SCSV_ROW_MISSING_NEW_LINE:
            s = "Row is missing a new line character";
        break;

        case SCSV_INVALID_ESCAPE_CODE:
            s = "Invalid escape code";
        break;

        case SCSV_MISSING_ESCAPE_CODE_END:
            s = "Missing escape code ending";
        break;

        default:
            s = "An unknown CSV load error occured!";
    }

    return s;
}

}

