#include "math.h"
#include "read.h"
#include "write.h"
#include <stdexcept>

int main() {
    writelib::writeLine("=== C++ Maven Calculator ===");
    writelib::writeLine("Operations: sum, subtract, multiply, divide");
    writelib::writeLine("");

    double a = readlib::readDouble("Enter first number: ");
    double b = readlib::readDouble("Enter second number: ");

    writelib::writeLine("");
    writelib::writeResult("Sum", mathlib::sum(a, b));
    writelib::writeResult("Subtract", mathlib::subtract(a, b));
    writelib::writeResult("Multiply", mathlib::multiply(a, b));

    try {
        writelib::writeResult("Divide", mathlib::divide(a, b));
    } catch (const std::invalid_argument& e) {
        writelib::writeError(e.what());
    }

    return 0;
}
