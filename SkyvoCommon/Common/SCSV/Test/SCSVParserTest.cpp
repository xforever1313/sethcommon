#include <boost/test/unit_test.hpp>

#define private public

#include "SCSVConstants.h"
#include "SCSVParser.h"
#include "TestHelper.h"

///\brief tests default construction
BOOST_AUTO_TEST_CASE(CSVParser_defaultConstructionTest){
    Common::SCSVParser uut;
    BOOST_CHECK_EQUAL(uut.m_space_filler, SCSV_DEFAULT_STRING);

    uut.getErrorMessage(Common::SCSV_DEFAULT); //get full coverage by calling default value
}

///\brief tests the case where there are no missing values.
BOOST_AUTO_TEST_CASE(CSVParser_goodIntFileTest){
    Common::SCSVParser uut(SCSV_DEFAULT_STRING);
    Common::SCSVParser::SCSVFileStatus_t status = uut.parseCsvFile(csvFileDirectory + "/goodIntFile.scsv");

    BOOST_CHECK_EQUAL(status.errorNumber, Common::SCSV_OKAY);

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
    Common::SCSVParser uut(SCSV_DEFAULT_STRING);
    Common::SCSVParser::SCSVFileStatus_t status = uut.parseCsvFile(csvFileDirectory + "/goodStringWithSpace.scsv");

    BOOST_CHECK_EQUAL(status.errorNumber, Common::SCSV_OKAY);

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
    Common::SCSVParser uut("0");
    Common::SCSVParser::SCSVFileStatus_t status = uut.parseCsvFile(csvFileDirectory + "/goodIntFileMissingValues.scsv");

    BOOST_CHECK_EQUAL(status.errorNumber, Common::SCSV_OKAY);

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
    Common::SCSVParser uut("0");
    Common::SCSVParser::SCSVFileStatus_t status = uut.parseCsvFile(csvFileDirectory + "/goodIntFileBlankLine.scsv");

    BOOST_CHECK_EQUAL(status.errorNumber, Common::SCSV_OKAY);

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
    Common::SCSVParser uut(SCSV_DEFAULT_STRING);
    Common::SCSVParser::SCSVFileStatus_t status = uut.parseCsvFile(csvFileDirectory + "/goodMisMatchedRows.scsv");

    BOOST_CHECK_EQUAL(status.errorNumber, Common::SCSV_OKAY);

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
    Common::SCSVParser uut("hi");
    Common::SCSVParser::SCSVFileStatus_t status = uut.parseCsvFile(csvFileDirectory + "/goodJustCommas.scsv");

    BOOST_CHECK_EQUAL(status.errorNumber, Common::SCSV_OKAY);

    //Check the file contents
    for (unsigned int i = 0; i < status.CSVValues.size(); ++i){
        for (unsigned int j = 0; j < status.CSVValues[i].size(); ++j){
            BOOST_CHECK_EQUAL(status.CSVValues[i][j], "hi");
        }
    }
}

///\brief tests the case where there are missing values with strings.
BOOST_AUTO_TEST_CASE(SCSVParser_goodStringFileMissingValuesTest){
    Common::SCSVParser uut("0");
    Common::SCSVParser::SCSVFileStatus_t status = uut.parseCsvFile(csvFileDirectory + "/goodIntFileMissingValues.scsv");

    BOOST_CHECK_EQUAL(status.errorNumber, Common::SCSV_OKAY);

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
    Common::SCSVParser uut(SCSV_DEFAULT_STRING);
    Common::SCSVParser::SCSVFileStatus_t status = uut.parseCsvFile(csvFileDirectory + "/blankFile.scsv");

    BOOST_CHECK_EQUAL(status.errorNumber, Common::SCSV_OKAY);

    BOOST_CHECK_EQUAL(status.CSVValues.size(), static_cast<unsigned int>(0));
}

///\brief tests the case where there is no blank line at the end of the file
BOOST_AUTO_TEST_CASE(SCSVParser_missingNewLineTest){
    Common::SCSVParser uut(SCSV_DEFAULT_STRING);
    Common::SCSVParser::SCSVFileStatus_t status = uut.parseCsvFile(csvFileDirectory + "/noNewLine.scsv");

    BOOST_CHECK_EQUAL(status.errorNumber, Common::SCSV_ROW_MISSING_NEW_LINE);
}

///\brief tests the case where there is no blank line at the end of the file, and the file is just commas
BOOST_AUTO_TEST_CASE(SCSVParser_justCommasFileMissingNewLineTest){
    Common::SCSVParser uut(SCSV_DEFAULT_STRING);
    Common::SCSVParser::SCSVFileStatus_t status = uut.parseCsvFile(csvFileDirectory + "/justCommasNoNewLine.scsv");

    BOOST_CHECK_EQUAL(status.errorNumber, Common::SCSV_ROW_MISSING_NEW_LINE);
}

///\brief tests the case where the file does not exist
BOOST_AUTO_TEST_CASE(SCSVParser_fileDNETest){
    Common::SCSVParser uut(SCSV_DEFAULT_STRING);
    Common::SCSVParser::SCSVFileStatus_t status = uut.parseCsvFile("DERP.txt");

    BOOST_CHECK_EQUAL(status.errorNumber, Common::SCSV_OPEN_ERR);
}

///\brief tests the case where the file gets corrupted
BOOST_AUTO_TEST_CASE(SCSVParser_fileCorruptionTest){
    Common::SCSVParser uut(SCSV_DEFAULT_STRING);
    uut.m_corrupt_file = true;
    Common::SCSVParser::SCSVFileStatus_t status = uut.parseCsvFile(csvFileDirectory + "/goodIntFile.scsv");
    BOOST_CHECK_EQUAL(status.errorNumber, Common::SCSV_CORRUPTED_FILE);
}

///\brief checks to make sure the given exception is thrown when convertEscapeCodes is called
void testConvertEscapeCodesException(std::string stringToTest, Common::SCSVLoadErrors errorToCatch){
    Common::SCSVParser uut;
    try{
        uut.convertEscapeCodes(stringToTest);
        BOOST_CHECK(false);
    }
    catch(const Common::SCSVLoadErrors &e){
        BOOST_CHECK_EQUAL(errorToCatch, e);
    }
}

///\brief test the convertEscapeCodes method
BOOST_AUTO_TEST_CASE(SCSVParser_convertEscapeCodesTest){
    Common::SCSVParser uut;

    //Test a good case
    std::string ret;
    BOOST_CHECK_NO_THROW(ret = uut.convertEscapeCodes("&comma; &amp;;"));
    BOOST_CHECK_EQUAL(ret, ", &;");

    //Test bad cases

    //Missing ';'
    testConvertEscapeCodesException("&comma", Common::SCSV_MISSING_ESCAPE_CODE_END);
    testConvertEscapeCodesException("&comma hello world", Common::SCSV_MISSING_ESCAPE_CODE_END);

    //'\n' inside of an escape sequence
    testConvertEscapeCodesException("&comma\n;", Common::SCSV_MISSING_ESCAPE_CODE_END);
    testConvertEscapeCodesException("&comma \n hello world", Common::SCSV_MISSING_ESCAPE_CODE_END);

    //'&' inside of an escape sequence
    testConvertEscapeCodesException("&comma&;", Common::SCSV_MISSING_ESCAPE_CODE_END);
    testConvertEscapeCodesException("&comma & hello world", Common::SCSV_MISSING_ESCAPE_CODE_END);

    //','inside of an escape sequence
    testConvertEscapeCodesException("&comma,;", Common::SCSV_MISSING_ESCAPE_CODE_END);
    testConvertEscapeCodesException("&comma , hello world", Common::SCSV_MISSING_ESCAPE_CODE_END);

    //Invaid escape code
    testConvertEscapeCodesException("&derp; hello", Common::SCSV_INVALID_ESCAPE_CODE);
}

///\brief tests the getErrorMessage class (really only used for coverage)
BOOST_AUTO_TEST_CASE(SCSVParser_getErrorMessageTest){
    Common::SCSVParser uut;

    //Ensure the okay message returns an empty string
    BOOST_CHECK_EQUAL(uut.getErrorMessage(Common::SCSV_OKAY), "");

    for (int e = static_cast<int>(Common::SCSV_OKAY) + 1; e != Common::SCSV_DEFAULT; ++e){
        BOOST_CHECK(uut.getErrorMessage(static_cast<Common::SCSVLoadErrors>(e)) != "");
    }
}
