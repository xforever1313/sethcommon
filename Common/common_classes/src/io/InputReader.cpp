
//          Copyright Seth Hendrick 2014
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "io/ConsoleLogger.h"
#include "io/InputReader.h"
#include "io/LoggerBase.h"

namespace Common {
namespace IO {

std::string InputReader::PromptForInput(const std::string &promptMessage,
                                        std::istream &inputStream /*= std::cin*/,
                                        LoggerBase &outputLogger /*= ConsoleLogger::out*/) {

    outputLogger.write(promptMessage);

    std::string input;
    std::getline(inputStream, input);
    return input;
}

}
}
