#ifndef WRITE_H
#define WRITE_H

#include <string>

namespace writelib {
    void writeLine(const std::string& message);
    void writeResult(const std::string& operation, double result);
    void writeError(const std::string& message);
}

#endif // WRITE_H
