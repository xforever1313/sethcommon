#ifndef CSVPARSER_H_INCLUDED
#define CSVPARSER_H_INCLUDED

#include <cstring>
#include <fstream>
#ifdef UNIT_TEST
    #include <iostream>
#endif
#include <sstream>
#include <string>
#include <vector>

#include "SCSVConstants.h"

namespace SCSV{

class SCSVParser{

    public:
        ///\brief the status of parsing a file
        typedef struct SCSVFileStatus{
            SCSVLoadErrors errorNumber;         ///<The error number (CSV_OKAY if no error)
            std::string errorMessage;      ///<The error message ("" if no error)
            std::vector<std::vector <std::string> >  CSVValues; ///<Empty if error
        }SCSVFileStatus_t;

        ///\brief creates a SCSVParser with the default empty cell set at ""
        SCSVParser();

        ///\param - spaceFiller what to fill an empty cell with
        SCSVParser(const std::string &spaceFiller);

        virtual ~SCSVParser();

        ///\param csvFileLocation - path, relitive to exe file, to the csv file
        SCSVFileStatus_t parseCsvFile(const std::string &csvFileLocation) const;

    private:
        std::string getLine(std::ifstream &inFile) const;

        std::vector<std::string> parseCSVLine(const std::string &csvLine) const;

        SCSVFileStatus_t generateErrorStatus(const SCSVLoadErrors &errorNum) const;

        ///\brief converts any strings with &escapeCode; to the proper form
        std::string convertEscapeCodes(const std::string &s) const;

        std::string m_space_filler;

        #ifdef UNIT_TEST
            bool m_corrupt_file;
        #endif
};

}
#endif // CSVPARSER_H_INCLUDED
