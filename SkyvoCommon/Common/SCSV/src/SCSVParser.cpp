#include "SCSVParser.h"

namespace SCSV{

SCSVParser::SCSVParser() :
    SCSVParser(SCSV_DEFAULT_STRING)
{
}

SCSVParser::SCSVParser(std::string space_filler) :
    m_space_filler(space_filler)
{
    #ifdef UNIT_TEST
        m_corrupt_file = false;
    #endif
}

SCSVParser::~SCSVParser(){
}


SCSVParser::SCSVFileStatus_t SCSVParser::parseCsvFile(std::string csvFileLocation) const{

    SCSVFileStatus_t status;

    std::vector<std::vector <std::string> > cells;
    std::ifstream inFile;
    try{
        inFile.open(csvFileLocation.c_str());
        inFile.peek();
        if (inFile.fail()){
            throw SCSV_OPEN_ERR;
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

            throw SCSV_CORRUPTED_FILE;
        }
        inFile.close();
        status = generateErrorStatus(SCSV_OKAY);
        status.CSVValues = cells;
    }
    catch (const SCSVLoadErrors &e){
        status = generateErrorStatus(e);
    }

    return status;
}

std::string SCSVParser::getLine(std::ifstream &inFile) const{
    std::stringstream ss;
    while (inFile.peek() != '\n' && !inFile.eof() && !inFile.fail()){

        ss.put(inFile.get());
    }
    ss.put(inFile.get()); //keep \n

    return ss.str();
}

std::vector<std::string> SCSVParser::parseCSVLine(std::string csvLine) const{
    std::vector<std::string> ret;

    unsigned int firstIndex = 0;
    unsigned int secondIndex = 0;

    if (csvLine.back() != '\n'){
        throw SCSV_ROW_MISSING_NEW_LINE;
    }

    for (unsigned int i = 0; i < csvLine.size(); ++i){
        ///Walk the string for comma or new line, or the end of string
        if((csvLine[i] == SCSV_SEPARATOR) || (csvLine[i] == '\n')){
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

SCSVParser::SCSVFileStatus_t SCSVParser::generateErrorStatus(SCSVLoadErrors errorNum) const{
    SCSVFileStatus_t status;
    status.errorNumber = errorNum;
    status.errorMessage = getErrorMessage(errorNum);

    return status;
}

std::string SCSVParser::convertEscapeCodes(std::string s) const{

    std::stringstream ss;

    for (unsigned int i = 0; i < s.size(); ++i){
        //If an escape cahracter, convert to regular char
        if (s[i] == SCSV_ESCAPE_CHAR){
            unsigned int escapeBegin = i;
            unsigned int escapeEnd = 0;

            //Move on to the next character
            ++i;
            ++escapeEnd;

            //iterate through the string until a ',', ';', '\n', or the end of string is reached
            while ((i < s.size()) && (s[i] != SCSV_SEPARATOR) && (s[i] != SCSV_AMP) && (s[i] != SCSV_ESCAPE_END) && (s[i] != '\n')){
                ++i;
                ++escapeEnd;
            }

            //If we reached the end of the string before hitting ';', throw
            if (i == s.size()){
                throw SCSV_MISSING_ESCAPE_CODE_END;
            }
            switch (s[i]){
                //Can't have a separator within an escape sequence, throw
                case SCSV_SEPARATOR:
                    throw SCSV_MISSING_ESCAPE_CODE_END;

                //Can't have an amp within an escape sequence, throw
                case SCSV_AMP:
                    throw SCSV_MISSING_ESCAPE_CODE_END;

                //Can't have a new line within an escape sequence, throw
                case '\n':
                    throw SCSV_MISSING_ESCAPE_CODE_END;

                //Otherwise, convert escape code to a char
                default:
                    std::string escapeCode = s.substr(escapeBegin, escapeEnd + 1);
                    if (escapeCode == std::string(SCSV_SEPARATOR_REPLACE)){
                        ss << SCSV_SEPARATOR;
                    }
                    else if (escapeCode == std::string(SCSV_AMP_REPLACE)){
                        ss << SCSV_AMP;
                    }
                    else{
                        throw SCSV_INVALID_ESCAPE_CODE;
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

std::string SCSVParser::getErrorMessage(SCSVLoadErrors errorNum) const{
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
