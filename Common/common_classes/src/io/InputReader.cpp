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
