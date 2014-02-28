
//          Copyright Seth Hendrick 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef SCSVCHECKER_H_INCLUDED
#define SCSVCHECKER_H_INCLUDED

#include <string>

namespace SCSV{

///\brief Checks and replaces strings that are invalid in a SCSV file such as commasa and amps
class SCSVChecker{

    public:

        static std::string checkText(std::string s);

};

}
#endif // SCSVCHECKER_H_INCLUDED
