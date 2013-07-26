#include <cctype>
#include <string>

#include "StringOps.h"

namespace Common{

void StringOps::toLowerCaseModify(std::string &s){
    for (unsigned int i = 0; i < s.size(); ++i){
        s[i] = ::tolower(s[i]);
    }
}

void StringOps::toUpperCaseModify(std::string &s){
    for (unsigned int i = 0; i < s.size(); ++i){
        s[i] = ::toupper(s[i]);
    }
}

std::string StringOps::toLowerCaseCopy(std::string s){
    toLowerCaseModify(s);
    return s;
}

std::string StringOps::toUpperCaseCopy(std::string s){
    toUpperCaseModify(s);
    return s;
}

void StringOps::removeWhiteSpaceModify(std::string &s){
    for (unsigned int i = 0; i < s.size(); ++i){
        if (::isspace(s[i]) != 0){
            s.erase(i, 1);
            --i;
        }
    }
}

std::string StringOps::removeWhiteSpaceCopy(std::string s){
    removeWhiteSpaceModify(s);
    return s;
}

void StringOps::replaceTabsWithSpacesModify(std::string &s){
    const std::string replacementString("    ");
    for (unsigned int i = 0; i < s.size(); ++i){
        if (s[i] == '\t'){
            s.erase(i,1);
            s.insert(i, replacementString);
            i += replacementString.size() - 1;
        }
    }
}

std::string StringOps::replaceTabsWithSpacesCopy(std::string s){
    replaceTabsWithSpacesModify(s);
    return s;
}

}
