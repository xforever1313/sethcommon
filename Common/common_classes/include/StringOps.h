
//          Copyright Seth Hendrick 2014
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef STRINGOPS_H
#define STRINGOPS_H

#include <deque>
#include <list>
#include <string>
#include <vector>

namespace Common{

class StringOps
{
    public:
        /**
        * \brief Modifies the passed in string such that all upper characters go to lowercase
        */
        static void toLowerCaseModify(std::string &s);

        /**
        * \brief Modifies the passed in string such that all lower characters go to uppercase
        */
        static void toUpperCaseModify(std::string &s);

        /**
        * \brief Creates a copy of the string, and returns a string will all characters changed to lowercase.
        * The passed in string is not modified.
        * \return the lower cased string.
        */
        static std::string toLowerCaseCopy(std::string s);

        /**
        * \brief Creates a copy of the string, and returns a string will all characters changed to uppercase.
        * The passed in string is not modified.
        * \return the upper cased string.
        */
        static std::string toUpperCaseCopy(std::string s);

        /**
        * \brief removes whitespace from the given string.  The passed in string is modified
        */
        static void removeWhiteSpaceModify(std::string &s);

        /**
        * \brief removes whitespace from the given string, and returns a copy
        * \return a string with the whitespace removed from s
        */
        static std::string removeWhiteSpaceCopy(std::string s);

        /**
        * \brief removes tabs and replaces it with 4 spaces.  The passed in string is modified
        */
        static void replaceTabsWithSpacesModify(std::string &s);

        /**
        * \brief removes tabs and replaces it with 4 spaces and returns a copy.
        * The passed in string is not modified
        * \return a string with tabs replaced with whitespace from s
        */
        static std::string replaceTabsWithSpacesCopy(std::string s);

        /**
        * \brief takes a vector of strings, and puts it in alphabetical order.  This modifies the passed in vector
        */
        static void toAlphabeticalOrderModify(std::vector<std::string> &v);

        /**
        * \brief takes a vector of strings, and puts it in alphabetical order.  Does not modify the passed in vector
        * \return a vector in alphabetical order
        */
        static std::vector<std::string> toAlphabeticalOrderCopy(std::vector<std::string> v);

        /**
        * \brief takes a deque of strings, and puts it in alphabetical order.  This modifies the passed in deque
        */
        static void toAlphabeticalOrderModify(std::deque<std::string> &d);

        /**
        * \brief takes a deque of strings, and puts it in alphabetical order.  Does not modify the passed in deque
        * \return a deque in alphabetical order
        */
        static std::deque<std::string> toAlphabeticalOrderCopy(std::deque<std::string> d);

        /**
        * \brief takes a list of strings, and puts it in alphabetical order.  This modifies the passed in list
        */
        static void toAlphabeticalOrderModify(std::list<std::string> &l);

        /**
        * \brief takes a list of strings, and puts it in alphabetical order.  Does not modify the passed in list
        * \return a list in alphabetical order
        */
        static std::list<std::string> toAlphabeticalOrderCopy(std::list<std::string> l);

    private:
        StringOps();
        virtual ~StringOps();
        static bool stringCompare(const std::string &s1, const std::string &s2);
};

}

#endif // STRINGOPS_H
