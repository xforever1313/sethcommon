
//          Copyright Seth Hendrick 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <string>

#include "SCSVChecker.h"
#include "SCSVConstants.h"

namespace SCSV {
std::string SCSVChecker::checkText(std::string s) {

    for (size_t i = 0; i < s.size(); ++i) {
        if (s[i] == SCSVConstants::SCSV_AMP) {
            s.erase(i, 1);
            s.insert(i, SCSVConstants::SCSV_AMP_REPLACE);
            i += SCSVConstants::SCSV_AMP_REPLACE.size() - 1;
        }
        else if (s[i] == SCSVConstants::SCSV_SEPARATOR) {
            s.erase(i, 1);
            s.insert(i, SCSVConstants::SCSV_SEPARATOR_REPLACE);
            i += SCSVConstants::SCSV_SEPARATOR_REPLACE.size() - 1;
        }
    }
    return s;
}
}
