#include <gtest/gtest.h>
#include "write.h"
#include <sstream>
#include <iostream>

class WriteTest : public ::testing::Test {
protected:
    std::streambuf* originalCout;
    std::streambuf* originalCerr;
    std::ostringstream capturedCout;
    std::ostringstream capturedCerr;

    void SetUp() override {
        originalCout = std::cout.rdbuf(capturedCout.rdbuf());
        originalCerr = std::cerr.rdbuf(capturedCerr.rdbuf());
    }

    void TearDown() override {
        std::cout.rdbuf(originalCout);
        std::cerr.rdbuf(originalCerr);
    }
};

TEST_F(WriteTest, WriteLineOutputsMessage) {
    writelib::writeLine("Hello, World!");
    EXPECT_EQ(capturedCout.str(), "Hello, World!\n");
}

TEST_F(WriteTest, WriteLineEmptyString) {
    writelib::writeLine("");
    EXPECT_EQ(capturedCout.str(), "\n");
}

TEST_F(WriteTest, WriteResultFormatsCorrectly) {
    writelib::writeResult("Sum", 42.0);
    EXPECT_NE(capturedCout.str().find("[RESULT]"), std::string::npos);
    EXPECT_NE(capturedCout.str().find("Sum"), std::string::npos);
    EXPECT_NE(capturedCout.str().find("42"), std::string::npos);
}

TEST_F(WriteTest, WriteResultWithDecimal) {
    writelib::writeResult("Divide", 3.5);
    EXPECT_NE(capturedCout.str().find("3.5"), std::string::npos);
}

TEST_F(WriteTest, WriteResultWithNegative) {
    writelib::writeResult("Subtract", -5.0);
    EXPECT_NE(capturedCout.str().find("-5"), std::string::npos);
}

TEST_F(WriteTest, WriteErrorGoesToStderr) {
    writelib::writeError("Something went wrong");
    EXPECT_EQ(capturedCout.str(), "");
    EXPECT_NE(capturedCerr.str().find("[ERROR]"), std::string::npos);
    EXPECT_NE(capturedCerr.str().find("Something went wrong"), std::string::npos);
}

TEST_F(WriteTest, WriteErrorFormatsCorrectly) {
    writelib::writeError("Division by zero");
    EXPECT_NE(capturedCerr.str().find("[ERROR] Division by zero"), std::string::npos);
}
