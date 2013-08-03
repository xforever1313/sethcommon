#include <boost/test/unit_test.hpp>

#define private public

#include "SCSVConstants.h"
#include "SCSVParser.h"
#include "TestHelper.h"

///\brief tests default construction
BOOST_AUTO_TEST_CASE(CSVParser_defaultConstructionTest){
    SCSV::SCSVParser uut;
    BOOST_CHECK_EQUAL(uut.m_space_filler, SCSV_DEFAULT_STRING);

    uut.getErrorMessage(SCSV::SCSV_DEFAULT); //get full coverage by calling default value
}

///\brief tests the case where there are no missing values.
BOOST_AUTO_TEST_CASE(CSVParser_goodIntFileTest){
    SCSV::SCSVParser uut(SCSV_DEFAULT_STRING);
    SCSV::SCSVParser::SCSVFileStatus_t status = uut.parseCsvFile(csvFileDirectory + "/goodIntFile.scsv");

    BOOST_CHECK_EQUAL(status.errorNumber, SCSV::SCSV_OKAY);

    //Check the file contents
    for (unsigned int i = 0; i < status.CSVValues.size(); ++i){
        for (unsigned int j = 0; j < status.CSVValues[i].size(); ++j){
            std::stringstream ss;
            ss << j+1;
            BOOST_CHECK_EQUAL(status.CSVValues[i][j], ss.str());
        }
    }
}

///\brief tests the case where there are spaces between values
BOOST_AUTO_TEST_CASE(SCSVParser_goodIntFileSpacesTest){
    SCSV::SCSVParser uut(SCSV_DEFAULT_STRING);
    SCSV::SCSVParser::SCSVFileStatus_t status = uut.parseCsvFile(csvFileDirectory + "/goodStringWithSpace.scsv");

    BOOST_CHECK_EQUAL(status.errorNumber, SCSV::SCSV_OKAY);

    //Check the file contents
    for (unsigned int i = 0; i < status.CSVValues.size(); ++i){
        for (unsigned int j = 0; j < status.CSVValues[i].size(); ++j){
            if (i == j){
                BOOST_CHECK_EQUAL(status.CSVValues[i][j], "& hello world! ,");
            }
            else{
                std::stringstream ss;
                ss << j+1;
                BOOST_CHECK_EQUAL(status.CSVValues[i][j], ss.str());
            }
        }
    }
}

///\brief tests the case where there are missing values.
BOOST_AUTO_TEST_CASE(SCSVParser_goodIntFileMissingValuesTest){
    SCSV::SCSVParser uut("0");
    SCSV::SCSVParser::SCSVFileStatus_t status = uut.parseCsvFile(csvFileDirectory + "/goodIntFileMissingValues.scsv");

    BOOST_CHECK_EQUAL(status.errorNumber, SCSV::SCSV_OKAY);

    //Check the file contents
    for (unsigned int i = 0; i < status.CSVValues.size(); ++i){
        for (unsigned int j = 0; j < status.CSVValues[i].size(); ++j){
            if (i == j){
                //Ensure diagonal rows are 0, the default missing value
                BOOST_CHECK_EQUAL(status.CSVValues[i][j], "0");
            }
            else{
                std::stringstream ss;
                ss << j+1;
                BOOST_CHECK_EQUAL(status.CSVValues[i][j], ss.str());
            }
        }
    }
}

///\brief tests the case where there are blank lines throughout the file
BOOST_AUTO_TEST_CASE(SCSVParser_goodIntFileBlanklineTest){
    SCSV::SCSVParser uut("0");
    SCSV::SCSVParser::SCSVFileStatus_t status = uut.parseCsvFile(csvFileDirectory + "/goodIntFileBlankLine.scsv");

    BOOST_CHECK_EQUAL(status.errorNumber, SCSV::SCSV_OKAY);

    //Check the file contents
    for (unsigned int i = 0; i < status.CSVValues.size(); ++i){
        for (unsigned int j = 0; j < status.CSVValues[i].size(); ++j){
            //For this test, the ones with one value in them are the blank lines
            if (status.CSVValues[i].size() == 1){
                std::stringstream ss;
                ss << 0;
                BOOST_CHECK_EQUAL(status.CSVValues[i][j], ss.str());
            }
            else{
                std::stringstream ss;
                ss << j+1;
                BOOST_CHECK_EQUAL(status.CSVValues[i][j], ss.str());
            }
        }
    }
}

///\brief tests the case where the rows do not match up
BOOST_AUTO_TEST_CASE(SCSVParser_misMatchingRows){
    SCSV::SCSVParser uut(SCSV_DEFAULT_STRING);
    SCSV::SCSVParser::SCSVFileStatus_t status = uut.parseCsvFile(csvFileDirectory + "/goodMisMatchedRows.scsv");

    BOOST_CHECK_EQUAL(status.errorNumber, SCSV::SCSV_OKAY);

    //Check the file contents
    for (unsigned int i = 0; i < status.CSVValues.size(); ++i){
        for (unsigned int j = 0; j < status.CSVValues[i].size(); ++j){
            std::stringstream ss;
            ss << j+1;
            BOOST_CHECK_EQUAL(status.CSVValues[i][j], ss.str());
        }
    }
}

///\brief tests the case where the file is just commas
BOOST_AUTO_TEST_CASE(SCSVParser_justCommaTest){
    SCSV::SCSVParser uut("hi");
    SCSV::SCSVParser::SCSVFileStatus_t status = uut.parseCsvFile(csvFileDirectory + "/goodJustCommas.scsv");

    BOOST_CHECK_EQUAL(status.errorNumber, SCSV::SCSV_OKAY);

    //Check the file contents
    for (unsigned int i = 0; i < status.CSVValues.size(); ++i){
        for (unsigned int j = 0; j < status.CSVValues[i].size(); ++j){
            BOOST_CHECK_EQUAL(status.CSVValues[i][j], "hi");
        }
    }
}

///\brief tests the case where there are missing values with strings.
BOOST_AUTO_TEST_CASE(SCSVParser_goodStringFileMissingValuesTest){
    SCSV::SCSVParser uut("0");
    SCSV::SCSVParser::SCSVFileStatus_t status = uut.parseCsvFile(csvFileDirectory + "/goodIntFileMissingValues.scsv");

    BOOST_CHECK_EQUAL(status.errorNumber, SCSV::SCSV_OKAY);

    //Check the file contents
    for (unsigned int i = 0; i < status.CSVValues.size(); ++i){
        for (unsigned int j = 0; j < status.CSVValues[i].size(); ++j){
            if (i == j){
                //Ensure diagonal rows are 0, the default missing value
                BOOST_CHECK_EQUAL(status.CSVValues[i][j], "0");
            }
            else{
                std::stringstream ss;
                ss << (j+1);
                BOOST_CHECK_EQUAL(status.CSVValues[i][j], ss.str());
            }
        }
    }
}

///\brief blank file test
BOOST_AUTO_TEST_CASE(SCSVParser_blankFileTest){
    SCSV::SCSVParser uut(SCSV_DEFAULT_STRING);
    SCSV::SCSVParser::SCSVFileStatus_t status = uut.parseCsvFile(csvFileDirectory + "/blankFile.scsv");

    BOOST_CHECK_EQUAL(status.errorNumber, SCSV::SCSV_OKAY);

    BOOST_CHECK_EQUAL(status.CSVValues.size(), static_cast<unsigned int>(0));
}

///\brief tests the case where there is no blank line at the end of the file
BOOST_AUTO_TEST_CASE(SCSVParser_missingNewLineTest){
    SCSV::SCSVParser uut(SCSV_DEFAULT_STRING);
    SCSV::SCSVParser::SCSVFileStatus_t status = uut.parseCsvFile(csvFileDirectory + "/noNewLine.scsv");

    BOOST_CHECK_EQUAL(status.errorNumber, SCSV::SCSV_ROW_MISSING_NEW_LINE);
}

///\brief tests the case where there is no blank line at the end of the file, and the file is just commas
BOOST_AUTO_TEST_CASE(SCSVParser_justCommasFileMissingNewLineTest){
    SCSV::SCSVParser uut(SCSV_DEFAULT_STRING);
    SCSV::SCSVParser::SCSVFileStatus_t status = uut.parseCsvFile(csvFileDirectory + "/justCommasNoNewLine.scsv");

    BOOST_CHECK_EQUAL(status.errorNumber, SCSV::SCSV_ROW_MISSING_NEW_LINE);
}

///\brief tests the case where the file does not exist
BOOST_AUTO_TEST_CASE(SCSVParser_fileDNETest){
    SCSV::SCSVParser uut(SCSV_DEFAULT_STRING);
    SCSV::SCSVParser::SCSVFileStatus_t status = uut.parseCsvFile("DERP.txt");

    BOOST_CHECK_EQUAL(status.errorNumber, SCSV::SCSV_OPEN_ERR);
}

///\brief tests the case where the file gets corrupted
BOOST_AUTO_TEST_CASE(SCSVParser_fileCorruptionTest){
    SCSV::SCSVParser uut(SCSV_DEFAULT_STRING);
    uut.m_corrupt_file = true;
    SCSV::SCSVParser::SCSVFileStatus_t status = uut.parseCsvFile(csvFileDirectory + "/goodIntFile.scsv");
    BOOST_CHECK_EQUAL(status.errorNumber, SCSV::SCSV_CORRUPTED_FILE);
}

///\brief checks to make sure the given exception is thrown when convertEscapeCodes is called
void testConvertEscapeCodesException(std::string stringToTest, SCSV::SCSVLoadErrors errorToCatch){
    SCSV::SCSVParser uut;
    try{
        uut.convertEscapeCodes(stringToTest);
        BOOST_CHECK(false);
    }
    catch(const SCSV::SCSVLoadErrors &e){
        BOOST_CHECK_EQUAL(errorToCatch, e);
    }
}

///\brief test the convertEscapeCodes method
BOOST_AUTO_TEST_CASE(SCSVParser_convertEscapeCodesTest){
    SCSV::SCSVParser uut;

    //Test a good case
    std::string ret;
    BOOST_CHECK_NO_THROW(ret = uut.convertEscapeCodes("&comma; &amp;;"));
    BOOST_CHECK_EQUAL(ret, ", &;");

    //Test bad cases

    //Missing ';'
    testConvertEscapeCodesException("&comma", SCSV::SCSV_MISSING_ESCAPE_CODE_END);
    testConvertEscapeCodesException("&comma hello world", SCSV::SCSV_MISSING_ESCAPE_CODE_END);

    //'\n' inside of an escape sequence
    testConvertEscapeCodesException("&comma\n;", SCSV::SCSV_MISSING_ESCAPE_CODE_END);
    testConvertEscapeCodesException("&comma \n hello world", SCSV::SCSV_MISSING_ESCAPE_CODE_END);

    //'&' inside of an escape sequence
    testConvertEscapeCodesException("&comma&;", SCSV::SCSV_MISSING_ESCAPE_CODE_END);
    testConvertEscapeCodesException("&comma & hello world", SCSV::SCSV_MISSING_ESCAPE_CODE_END);

    //','inside of an escape sequence
    testConvertEscapeCodesException("&comma,;", SCSV::SCSV_MISSING_ESCAPE_CODE_END);
    testConvertEscapeCodesException("&comma , hello world", SCSV::SCSV_MISSING_ESCAPE_CODE_END);

    //Invaid escape code
    testConvertEscapeCodesException("&derp; hello", SCSV::SCSV_INVALID_ESCAPE_CODE);
}

///\brief tests the getErrorMessage class (really only used for coverage)
BOOST_AUTO_TEST_CASE(SCSVParser_getErrorMessageTest){
    SCSV::SCSVParser uut;

    //Ensure the okay message returns an empty string
    BOOST_CHECK_EQUAL(uut.getErrorMessage(SCSV::SCSV_OKAY), "");

    for (int e = static_cast<int>(SCSV::SCSV_OKAY) + 1; e != SCSV::SCSV_DEFAULT; ++e){
        BOOST_CHECK(uut.getErrorMessage(static_cast<SCSV::SCSVLoadErrors>(e)) != "");
    }
}
