
//          Copyright Seth Hendrick 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <CppUTest/TestHarness.h>
#include <string>

#define private public

#include "SCSVConstants.h"
#include "SCSVException.h"
#include "SCSVParser.h"
#include "TestHelper.h"

TEST_GROUP(SCSVParser){
    TEST_SETUP() {
        m_uut = new SCSV::SCSVParser(SCSV::SCSVConstants::SCSV_DEFAULT_STRING);
    }
    
    TEST_TEARDOWN() {
        delete m_uut;
    }
    
    SCSV::SCSVParser *m_uut;

};

///\brief tests default construction
TEST(SCSVParser, defaultConstructionTest){
    SCSV::SCSVParser uut;
    CHECK_EQUAL(uut.m_space_filler, SCSV::SCSVConstants::SCSV_DEFAULT_STRING);
}

///\brief tests the case where there are no missing values.
TEST(SCSVParser, goodIntFileTest){
    SCSV::SCSVParser::SCSVFileStatus_t status = m_uut->parseCsvFile(csvFileDirectory + "/goodIntFile.scsv");

    CHECK_EQUAL(status.errorNumber, SCSV::SCSV_OKAY);

    //Check the file contents
    for (unsigned int i = 0; i < status.CSVValues.size(); ++i){
        for (unsigned int j = 0; j < status.CSVValues[i].size(); ++j){
            std::stringstream ss;
            ss << j+1;
            CHECK_EQUAL(status.CSVValues[i][j], ss.str());
        }
    }
}

///\brief tests the case where there are spaces between values
TEST(SCSVParser, goodIntFileSpacesTest){
    SCSV::SCSVParser::SCSVFileStatus_t status = m_uut->parseCsvFile(csvFileDirectory + "/goodStringWithSpace.scsv");

    CHECK_EQUAL(status.errorNumber, SCSV::SCSV_OKAY);

    //Check the file contents
    for (unsigned int i = 0; i < status.CSVValues.size(); ++i){
        for (unsigned int j = 0; j < status.CSVValues[i].size(); ++j){
            if (i == j){
                CHECK_EQUAL(status.CSVValues[i][j], "& hello world! ,");
            }
            else{
                std::stringstream ss;
                ss << j+1;
                CHECK_EQUAL(status.CSVValues[i][j], ss.str());
            }
        }
    }
}

///\brief tests the case where there are missing values.
TEST(SCSVParser, goodIntFileMissingValuesTest){
    SCSV::SCSVParser uut("0");
    SCSV::SCSVParser::SCSVFileStatus_t status = uut.parseCsvFile(csvFileDirectory + "/goodIntFileMissingValues.scsv");

    CHECK_EQUAL(status.errorNumber, SCSV::SCSV_OKAY);

    //Check the file contents
    for (unsigned int i = 0; i < status.CSVValues.size(); ++i){
        for (unsigned int j = 0; j < status.CSVValues[i].size(); ++j){
            if (i == j){
                //Ensure diagonal rows are 0, the default missing value
                CHECK_EQUAL(status.CSVValues[i][j], "0");
            }
            else{
                std::stringstream ss;
                ss << j+1;
                CHECK_EQUAL(status.CSVValues[i][j], ss.str());
            }
        }
    }
}

///\brief tests the case where there are blank lines throughout the file
TEST(SCSVParser, goodIntFileBlanklineTest){
    SCSV::SCSVParser uut("0");
    SCSV::SCSVParser::SCSVFileStatus_t status = uut.parseCsvFile(csvFileDirectory + "/goodIntFileBlankLine.scsv");

    CHECK_EQUAL(status.errorNumber, SCSV::SCSV_OKAY);

    //Check the file contents
    for (unsigned int i = 0; i < status.CSVValues.size(); ++i){
        for (unsigned int j = 0; j < status.CSVValues[i].size(); ++j){
            //For this test, the ones with one value in them are the blank lines
            if (status.CSVValues[i].size() == 1){
                std::stringstream ss;
                ss << 0;
                CHECK_EQUAL(status.CSVValues[i][j], ss.str());
            }
            else{
                std::stringstream ss;
                ss << j+1;
                CHECK_EQUAL(status.CSVValues[i][j], ss.str());
            }
        }
    }
}

///\brief tests the case where the rows do not match up
TEST(SCSVParser, misMatchingRows){
    SCSV::SCSVParser::SCSVFileStatus_t status = m_uut->parseCsvFile(csvFileDirectory + "/goodMisMatchedRows.scsv");

    CHECK_EQUAL(status.errorNumber, SCSV::SCSV_OKAY);

    //Check the file contents
    for (unsigned int i = 0; i < status.CSVValues.size(); ++i){
        for (unsigned int j = 0; j < status.CSVValues[i].size(); ++j){
            std::stringstream ss;
            ss << j+1;
            CHECK_EQUAL(status.CSVValues[i][j], ss.str());
        }
    }
}

///\brief tests the case where the file is just commas
TEST(SCSVParser, justCommaTest){
    SCSV::SCSVParser uut("hi");
    SCSV::SCSVParser::SCSVFileStatus_t status = uut.parseCsvFile(csvFileDirectory + "/goodJustCommas.scsv");

    CHECK_EQUAL(status.errorNumber, SCSV::SCSV_OKAY);

    //Check the file contents
    for (unsigned int i = 0; i < status.CSVValues.size(); ++i){
        for (unsigned int j = 0; j < status.CSVValues[i].size(); ++j){
            CHECK_EQUAL(status.CSVValues[i][j], "hi");
        }
    }
}

///\brief tests the case where there are missing values with strings.
TEST(SCSVParser, goodStringFileMissingValuesTest){
    SCSV::SCSVParser uut("0");
    SCSV::SCSVParser::SCSVFileStatus_t status = uut.parseCsvFile(csvFileDirectory + "/goodIntFileMissingValues.scsv");

    CHECK_EQUAL(status.errorNumber, SCSV::SCSV_OKAY);

    //Check the file contents
    for (unsigned int i = 0; i < status.CSVValues.size(); ++i){
        for (unsigned int j = 0; j < status.CSVValues[i].size(); ++j){
            if (i == j){
                //Ensure diagonal rows are 0, the default missing value
                CHECK_EQUAL(status.CSVValues[i][j], "0");
            }
            else{
                std::stringstream ss;
                ss << (j+1);
                CHECK_EQUAL(status.CSVValues[i][j], ss.str());
            }
        }
    }
}

///\brief blank file test
TEST(SCSVParser, blankFileTest){
    SCSV::SCSVParser::SCSVFileStatus_t status = m_uut->parseCsvFile(csvFileDirectory + "/blankFile.scsv");

    CHECK_EQUAL(status.errorNumber, SCSV::SCSV_OKAY);

    CHECK_EQUAL(status.CSVValues.size(), static_cast<size_t>(0));
}

///\brief tests the case where there is no blank line at the end of the file
TEST(SCSVParser, missingNewLineTest){
    SCSV::SCSVParser::SCSVFileStatus_t status = m_uut->parseCsvFile(csvFileDirectory + "/noNewLine.scsv");

    CHECK_EQUAL(status.errorNumber, SCSV::SCSV_ROW_MISSING_NEW_LINE);
}

///\brief tests the case where there is no blank line at the end of the file, and the file is just commas
TEST(SCSVParser, justCommasFileMissingNewLineTest){
    SCSV::SCSVParser::SCSVFileStatus_t status = m_uut->parseCsvFile(csvFileDirectory + "/justCommasNoNewLine.scsv");

    CHECK_EQUAL(status.errorNumber, SCSV::SCSV_ROW_MISSING_NEW_LINE);
}

///\brief tests the case where the file does not exist
TEST(SCSVParser, fileDNETest){
    SCSV::SCSVParser::SCSVFileStatus_t status = m_uut->parseCsvFile("DERP.txt");

    CHECK_EQUAL(status.errorNumber, SCSV::SCSV_OPEN_ERR);
}

///\brief tests the case where the file gets corrupted
TEST(SCSVParser, fileCorruptionTest){
    m_uut->m_corrupt_file = true;
    SCSV::SCSVParser::SCSVFileStatus_t status = m_uut->parseCsvFile(csvFileDirectory + "/goodIntFile.scsv");
    CHECK_EQUAL(status.errorNumber, SCSV::SCSV_CORRUPTED_FILE);
}

///\brief checks to make sure the given exception is thrown when convertEscapeCodes is called
void testConvertEscapeCodesException(std::string stringToTest, SCSV::SCSVLoadErrors errorToCatch){
    SCSV::SCSVParser uut;
    try{
        uut.convertEscapeCodes(stringToTest);
        CHECK(false);
    }
    catch(const SCSV::SCSVException &e){
        CHECK_EQUAL(errorToCatch, e.getError());
    }
}

///\brief test the convertEscapeCodes method
TEST(SCSVParser, convertEscapeCodesTest){
    SCSV::SCSVParser uut;

    //Test a good case
    std::string ret;
    try{
        ret = uut.convertEscapeCodes(std::string("&comma; &amp;;"));
    }
    catch(...){
        FAIL("Unhandled Exception");
    }
        
    CHECK_EQUAL(ret, ", &;");

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
