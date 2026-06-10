#ifndef READ_H
#define READ_H

#include <string>

namespace readlib {
    std::string readLine(const std::string& prompt);
    int readInt(const std::string& prompt);
    double readDouble(const std::string& prompt);
}

#endif // READ_H
