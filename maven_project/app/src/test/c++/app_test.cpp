#include <gtest/gtest.h>
#include "math.h"
#include "read.h"
#include "write.h"
#include <sstream>
#include <iostream>
#include <stdexcept>

class AppIntegrationTest : public ::testing::Test {
protected:
    std::streambuf* originalCin;
    std::streambuf* originalCout;
    std::streambuf* originalCerr;
    std::istringstream mockInput;
    std::ostringstream capturedCout;
    std::ostringstream capturedCerr;

    void SetUp() override {
        originalCout = std::cout.rdbuf(capturedCout.rdbuf());
        originalCerr = std::cerr.rdbuf(capturedCerr.rdbuf());
    }

    void TearDown() override {
        if (originalCin) {
            std::cin.rdbuf(originalCin);
        }
        std::cout.rdbuf(originalCout);
        std::cerr.rdbuf(originalCerr);
    }

    void setInput(const std::string& input) {
        mockInput.str(input);
        mockInput.clear();
        originalCin = std::cin.rdbuf(mockInput.rdbuf());
    }
};

TEST_F(AppIntegrationTest, MathAndWriteIntegration) {
    double a = 10.0, b = 5.0;
    writelib::writeResult("Sum", mathlib::sum(a, b));
    EXPECT_NE(capturedCout.str().find("15"), std::string::npos);
}

TEST_F(AppIntegrationTest, SubtractAndWriteIntegration) {
    double a = 10.0, b = 3.0;
    writelib::writeResult("Subtract", mathlib::subtract(a, b));
    EXPECT_NE(capturedCout.str().find("7"), std::string::npos);
}

TEST_F(AppIntegrationTest, MultiplyAndWriteIntegration) {
    double a = 4.0, b = 3.0;
    writelib::writeResult("Multiply", mathlib::multiply(a, b));
    EXPECT_NE(capturedCout.str().find("12"), std::string::npos);
}

TEST_F(AppIntegrationTest, DivideAndWriteIntegration) {
    double a = 10.0, b = 4.0;
    writelib::writeResult("Divide", mathlib::divide(a, b));
    EXPECT_NE(capturedCout.str().find("2.5"), std::string::npos);
}

TEST_F(AppIntegrationTest, DivisionByZeroErrorHandling) {
    double a = 5.0, b = 0.0;
    try {
        writelib::writeResult("Divide", mathlib::divide(a, b));
    } catch (const std::invalid_argument& e) {
        writelib::writeError(e.what());
    }
    EXPECT_NE(capturedCerr.str().find("[ERROR]"), std::string::npos);
    EXPECT_NE(capturedCerr.str().find("Division by zero"), std::string::npos);
}

TEST_F(AppIntegrationTest, ReadAndComputeIntegration) {
    setInput("6.0\n3.0\n");
    double a = readlib::readDouble("First: ");
    double b = readlib::readDouble("Second: ");
    writelib::writeResult("Sum", mathlib::sum(a, b));
    EXPECT_NE(capturedCout.str().find("9"), std::string::npos);
}

TEST_F(AppIntegrationTest, FullWorkflowWithAllOperations) {
    setInput("8.0\n2.0\n");
    double a = readlib::readDouble("A: ");
    double b = readlib::readDouble("B: ");

    writelib::writeResult("Sum", mathlib::sum(a, b));
    writelib::writeResult("Subtract", mathlib::subtract(a, b));
    writelib::writeResult("Multiply", mathlib::multiply(a, b));
    writelib::writeResult("Divide", mathlib::divide(a, b));

    std::string output = capturedCout.str();
    EXPECT_NE(output.find("10"), std::string::npos);   // 8+2
    EXPECT_NE(output.find("6"), std::string::npos);    // 8-2
    EXPECT_NE(output.find("16"), std::string::npos);   // 8*2
    EXPECT_NE(output.find("4"), std::string::npos);    // 8/2
}
