#include <string>

#include "SCSVChecker.h"
#include "SCSVConstants.h"

namespace SCSV{
std::string SCSVChecker::checkText(std::string s){

    for (unsigned int i = 0; i < s.size(); ++i){
        if(s[i] == SCSV_AMP){
            s.erase(i, 1);
            s.insert(i, SCSV_AMP_REPLACE);
            i += std::string(SCSV_AMP_REPLACE).size()-1;
        }
        else if (s[i] == SCSV_SEPARATOR){
            s.erase(i, 1);
            s.insert(i, SCSV_SEPARATOR_REPLACE);
            i += std::string(SCSV_SEPARATOR_REPLACE).size()-1;
        }
    }
    return s;
}
}
