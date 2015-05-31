
//          Copyright Seth Hendrick 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef SCSV_EXCEPTION_H_
#define SCSV_EXCEPTION_H_

#include <exception>
#include <string>

#include "SCSVConstants.h"

namespace SCSV{

class SCSVException : public std::exception{
    public:
		explicit SCSVException(const SCSVLoadErrors &e);

		virtual ~SCSVException();

		SCSVLoadErrors getError() const;

		virtual const char* what() const throw() override;

#ifndef UNIT_TEST //This is here since otherwise MSVC throws up a linker error
    private: 
#endif
		SCSVException();

		static std::string getErrorMessage(const SCSVLoadErrors &e);

		SCSVLoadErrors m_error;

		std::string m_errorMessage;
};

}

#endif

