#ifndef SCSV_EXCEPTION_H_
#define SCSV_EXCEPTION_H_

#include <exception>
#include <string>

#include "SCSVConstants.h"

namespace SCSV{

class SCSVException : public std::exception{
    public:
	SCSVException(const SCSVLoadErrors &e);

	virtual ~SCSVException();

	SCSVLoadErrors getError() const;

	const char* what() const throw() override;

    private:
	SCSVException();

	static std::string getErrorMessage(const SCSVLoadErrors &e);

	SCSVLoadErrors m_error;

	std::string m_errorMessage;
};

}

#endif

