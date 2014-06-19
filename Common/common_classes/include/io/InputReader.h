
//          Copyright Seth Hendrick 2014
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef INPUTREADER_H
#define INPUTREADER_H

#include <iostream>
#include <string>

#include "io/ConsoleLogger.h"
#include "io/LoggerBase.h"

namespace Common {
namespace IO {

class InputReader {
    public:
        /**
         * \brief The purpose of this method is to give the user a message to the console
         *        and then receive input from the user.  This is the equivalent of:
         *        std::cout << promptMessage; std::cout.flush(); std::getline(std::cin, str); return str;
         *
         * \param promptMessage The message printed to the user before asking for Input.  It prints EXACTLY
         *                      what is put in, so if you want a space or a newline between the message
         *                      and the user input, ensure you put it in!
         *
         * \param inputStream Defaulted to std::cin.  Only to be used for testing purposes.
         * \param outputStream Defaulted to std::cout.  Only to be used for testing purposes.
         */
        static std::string PromptForInput(const std::string &promptMessage,
                                          std::istream &inputStream = std::cin,
                                          LoggerBase &outputLogger = ConsoleLogger::out);

    private:
        InputReader() = delete;
        InputReader(const InputReader&) = delete;
        InputReader(const InputReader&&) = delete;
        InputReader &operator=(const InputReader&) = delete;
        InputReader &operator=(const InputReader&&) = delete;
};

}
}

#endif
