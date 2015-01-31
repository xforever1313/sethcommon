
//          Copyright Seth Hendrick 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <string>

#include "SCSVException.h"
#include "SCSVParser.h"

namespace SCSV{

SCSVParser::SCSVParser(const std::string &space_filler /* = SCSVConstants::SCSV_DEFAULT_STRING*/) :
    m_space_filler(space_filler)
{
#ifdef UNIT_TEST
    m_corrupt_file = false;
#endif
}

SCSVParser::~SCSVParser(){
}


SCSVParser::SCSVFileStatus_t SCSVParser::parseCsvFile(const std::string &csvFileLocation) const{

    SCSVFileStatus_t status;

    std::vector<std::vector <std::string> > cells;
    std::ifstream inFile;
    try{
        inFile.open(csvFileLocation.c_str());
        inFile.peek();
        if (inFile.fail()){
            throw SCSVException(SCSV_OPEN_ERR);
        }
        while (!inFile.fail() && !inFile.eof()){
            std::string csvLine = getLine(inFile);
            //If its a blank string, ignore the shit out of it.
            if (csvLine != ""){
                std::vector<std::string> tableRow = parseCSVLine(csvLine);
                cells.push_back(tableRow);
            }
            if (!inFile.eof()){
                inFile.peek();
            }
        }

        if (inFile.fail()
        #ifdef UNIT_TEST
        || m_corrupt_file
        #endif
        ){
            #ifdef UNIT_TEST
            inFile.close();
            #endif

            throw SCSVException(SCSV_CORRUPTED_FILE);
        }
        inFile.close();
        status.errorNumber = SCSV_OKAY;
        status.errorMessage = "";
        status.CSVValues = cells;
    }
    catch (const SCSVException &e){
        status.errorNumber = e.getError();
        status.errorMessage = std::string(e.what());
    }

    return status;
}

std::string SCSVParser::getLine(std::ifstream &inFile) const{
    std::stringstream ss;
    while (inFile.peek() != '\n' && !inFile.eof() && !inFile.fail()){
        char c = inFile.get();
        
        #ifdef ASM_JS
        //compiling with emscripten with files preloaded causes all \n to have \r in front of it
        if (c != '\r'){  //ignore cariage returns
        #endif
            ss.put(c);
        
        #ifdef ASM_JS
        }
        #endif
    }
    ss.put(inFile.get()); //keep \n

    return ss.str();
}

std::vector<std::string> SCSVParser::parseCSVLine(const std::string &csvLine) const{
    std::vector<std::string> ret;

    size_t firstIndex = 0;
    size_t secondIndex = 0;

    if (csvLine.back() != '\n'){
        throw SCSVException(SCSV_ROW_MISSING_NEW_LINE);
    }

    for (size_t i = 0; i < csvLine.size(); ++i){
        ///Walk the string for comma or new line, or the end of string
        if ((csvLine[i] == SCSVConstants::SCSV_SEPARATOR) || (csvLine[i] == '\n')){
            secondIndex = i + 1;  //Skip the comma or whitespace
            std::string column = csvLine.substr(firstIndex, i-firstIndex);
            if (column == ""){
                ret.push_back(m_space_filler);
            }
            else{
                ret.push_back(convertEscapeCodes(column));
            }
            firstIndex = secondIndex;
        }
    }

    return ret;
}

std::string SCSVParser::convertEscapeCodes(const std::string &s) const{

    std::stringstream ss;

    for (size_t i = 0; i < s.size(); ++i){
        //If an escape cahracter, convert to regular char
		if (s[i] == SCSVConstants::SCSV_ESCAPE_CHAR){
            size_t escapeBegin = i;
            size_t escapeEnd = 0;

            //Move on to the next character
            ++i;
            ++escapeEnd;

            //iterate through the string until a ',', ';', '\n', or the end of string is reached
            while ((i < s.size()) && (s[i] != SCSVConstants::SCSV_SEPARATOR) && (s[i] != SCSVConstants::SCSV_AMP) && (s[i] != SCSVConstants::SCSV_ESCAPE_END) && (s[i] != '\n')){
                ++i;
                ++escapeEnd;
            }

            //If we reached the end of the string before hitting ';', throw
            if (i == s.size()){
                throw SCSVException(SCSV_MISSING_ESCAPE_CODE_END);
            }
            switch (s[i]){
                //Can't have a separator within an escape sequence, throw
                case SCSVConstants::SCSV_SEPARATOR:
                    throw SCSVException(SCSV_MISSING_ESCAPE_CODE_END);

                //Can't have an amp within an escape sequence, throw
                case SCSVConstants::SCSV_AMP:
                    throw SCSVException(SCSV_MISSING_ESCAPE_CODE_END);

                //Can't have a new line within an escape sequence, throw
                case '\n':
                    throw SCSVException(SCSV_MISSING_ESCAPE_CODE_END);

                //Otherwise, convert escape code to a char
                default:
                    std::string escapeCode = s.substr(escapeBegin, escapeEnd + 1);
                    if (escapeCode == std::string(SCSVConstants::SCSV_SEPARATOR_REPLACE)){
                        ss << SCSVConstants::SCSV_SEPARATOR;
                    }
                    else if (escapeCode == std::string(SCSVConstants::SCSV_AMP_REPLACE)){
                        ss << SCSVConstants::SCSV_AMP;
                    }
                    else{
                        throw SCSVException(SCSV_INVALID_ESCAPE_CODE);
                    }
            }
        }
        //Otherwise, just push it back
        else{
            ss << s[i];
        }
    }

    return ss.str();
}

}
