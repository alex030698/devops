#include "write.h"
#include <iostream>

namespace writelib {

    void writeLine(const std::string& message) {
        std::cout << message << std::endl;
    }

    void writeResult(const std::string& operation, double result) {
        std::cout << "[RESULT] " << operation << " = " << result << std::endl;
    }

    void writeError(const std::string& message) {
        std::cerr << "[ERROR] " << message << std::endl;
    }

}
