#include <iostream>

#include "io/ConsoleLogger.h"
#include "io/LoggerBase.h"

namespace Common {
namespace IO {

LoggerBase ConsoleLogger::out(std::cout);
LoggerBase ConsoleLogger::err(std::cerr);

}
}
