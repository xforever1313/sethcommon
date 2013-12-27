#ifndef TESTHELPER_H_INCLUDED
#define TESTHELPER_H_INCLUDED

/**
* \file TestHelper.h
* \brief includes fucntions that multiple unit tests may need
* \author Seth Hendrick
*/

#ifdef ASM_JS
const std::string csvFileDirectory = "/Test/SCSVTestfiles";
#else
const std::string csvFileDirectory = "../../../Test/SCSVTestfiles";
#endif

#endif // TESTHELPER_H_INCLUDED