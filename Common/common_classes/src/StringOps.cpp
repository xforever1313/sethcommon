
//          Copyright Seth Hendrick 2014
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <algorithm>
#include <cctype>
#include <deque>
#include <list>
#include <string>
#include <vector>

#include "StringOps.h"

namespace Common {

void StringOps::toLowerCaseModify(std::string &s) {
    for (size_t i = 0; i < s.size(); ++i){
        s[i] = ::tolower(s[i]);
    }
}

void StringOps::toUpperCaseModify(std::string &s) {
    for (size_t i = 0; i < s.size(); ++i) {
        s[i] = ::toupper(s[i]);
    }
}

std::string StringOps::toLowerCaseCopy(std::string s) {
    toLowerCaseModify(s);
    return s;
}

std::string StringOps::toUpperCaseCopy(std::string s) {
    toUpperCaseModify(s);
    return s;
}

void StringOps::removeWhiteSpaceModify(std::string &s){
    for (size_t i = 0; i < s.size(); ++i){
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
    for (size_t i = 0; i < s.size(); ++i){
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

void StringOps::toAlphabeticalOrderModify(std::vector<std::string> &v){
    std::sort(v.begin(), v.end(), &StringOps::stringCompare);
}

std::vector<std::string> StringOps::toAlphabeticalOrderCopy(std::vector<std::string> v){
    toAlphabeticalOrderModify(v);
    return v;
}

void StringOps::toAlphabeticalOrderModify(std::deque<std::string> &d){
    std::sort(d.begin(), d.end(), &StringOps::stringCompare);
}

std::deque<std::string> StringOps::toAlphabeticalOrderCopy(std::deque<std::string> d){
    toAlphabeticalOrderModify(d);
    return d;
}

void StringOps::toAlphabeticalOrderModify(std::list<std::string> &l){
    l.sort(&stringCompare);
}

std::list<std::string> StringOps::toAlphabeticalOrderCopy(std::list<std::string> l){
    toAlphabeticalOrderModify(l);
    return l;
}

bool StringOps::stringCompare(const std::string &s1, const std::string &s2){
    bool lessThan = false;
    bool keepGoing = true;

    //First, check for letter order
    for (size_t i = 0; keepGoing && (i < s1.size()) && (i < s2.size()); ++i){
        if (::tolower(s1[i]) < ::tolower(s2[i])){
            keepGoing = false;
            lessThan = true;
        }
        else if (::tolower(s1[i]) > ::tolower(s2[i])){
            keepGoing = false;
            lessThan = false;
        }
    }

    //If all letters match, check size.
    if (keepGoing){
        if (s1.size() == s2.size()){ //If they are the same size, compare using capital letters
            lessThan = s1 < s2;
        }
        else{
            lessThan = (s1.size() < s2.size());  //Otherwise, compare by length.
        }
    }
    return lessThan;
}

}
