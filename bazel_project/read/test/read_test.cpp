#include <gtest/gtest.h>
#include "read.h"
#include <sstream>
#include <iostream>

class ReadTest : public ::testing::Test {
protected:
    std::streambuf* originalCin;
    std::streambuf* originalCout;
    std::istringstream mockInput;
    std::ostringstream mockOutput;

    void SetUp() override {
        originalCout = std::cout.rdbuf(mockOutput.rdbuf());
    }

    void TearDown() override {
        if (originalCin) {
            std::cin.rdbuf(originalCin);
        }
        std::cout.rdbuf(originalCout);
    }

    void setInput(const std::string& input) {
        mockInput.str(input);
        mockInput.clear();
        originalCin = std::cin.rdbuf(mockInput.rdbuf());
    }
};

TEST_F(ReadTest, ReadLineReturnsInput) {
    setInput("hello world\n");
    std::string result = readlib::readLine("Enter: ");
    EXPECT_EQ(result, "hello world");
}

TEST_F(ReadTest, ReadLineShowsPrompt) {
    setInput("test\n");
    readlib::readLine("Prompt> ");
    EXPECT_NE(mockOutput.str().find("Prompt> "), std::string::npos);
}

TEST_F(ReadTest, ReadIntReturnsInteger) {
    setInput("42\n");
    int result = readlib::readInt("Number: ");
    EXPECT_EQ(result, 42);
}

TEST_F(ReadTest, ReadIntNegativeNumber) {
    setInput("-10\n");
    int result = readlib::readInt("Number: ");
    EXPECT_EQ(result, -10);
}

TEST_F(ReadTest, ReadDoubleReturnsDecimal) {
    setInput("3.14\n");
    double result = readlib::readDouble("Value: ");
    EXPECT_DOUBLE_EQ(result, 3.14);
}

TEST_F(ReadTest, ReadDoubleNegativeNumber) {
    setInput("-2.5\n");
    double result = readlib::readDouble("Value: ");
    EXPECT_DOUBLE_EQ(result, -2.5);
}
