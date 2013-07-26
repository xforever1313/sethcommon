#ifndef STRINGOPS_H
#define STRINGOPS_H

#include <string>

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

    private:
        StringOps();
        virtual ~StringOps();
};

}

#endif // STRINGOPS_H
