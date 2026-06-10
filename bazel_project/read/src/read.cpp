#include "read.h"
#include <iostream>
#include <string>

namespace readlib {

    std::string readLine(const std::string& prompt) {
        std::cout << prompt;
        std::string input;
        std::getline(std::cin, input);
        return input;
    }

    int readInt(const std::string& prompt) {
        std::cout << prompt;
        int value;
        std::cin >> value;
        std::cin.ignore();
        return value;
    }

    double readDouble(const std::string& prompt) {
        std::cout << prompt;
        double value;
        std::cin >> value;
        std::cin.ignore();
        return value;
    }

}
