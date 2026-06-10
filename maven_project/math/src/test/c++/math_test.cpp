#include <gtest/gtest.h>
#include "math.h"

TEST(MathSumTest, PositiveNumbers) {
    EXPECT_DOUBLE_EQ(mathlib::sum(2.0, 3.0), 5.0);
}

TEST(MathSumTest, NegativeNumbers) {
    EXPECT_DOUBLE_EQ(mathlib::sum(-2.0, -3.0), -5.0);
}

TEST(MathSumTest, MixedNumbers) {
    EXPECT_DOUBLE_EQ(mathlib::sum(-2.0, 5.0), 3.0);
}

TEST(MathSumTest, Zeros) {
    EXPECT_DOUBLE_EQ(mathlib::sum(0.0, 0.0), 0.0);
}

TEST(MathSubtractTest, PositiveNumbers) {
    EXPECT_DOUBLE_EQ(mathlib::subtract(5.0, 3.0), 2.0);
}

TEST(MathSubtractTest, NegativeResult) {
    EXPECT_DOUBLE_EQ(mathlib::subtract(3.0, 5.0), -2.0);
}

TEST(MathSubtractTest, SameNumbers) {
    EXPECT_DOUBLE_EQ(mathlib::subtract(7.0, 7.0), 0.0);
}

TEST(MathMultiplyTest, PositiveNumbers) {
    EXPECT_DOUBLE_EQ(mathlib::multiply(3.0, 4.0), 12.0);
}

TEST(MathMultiplyTest, ByZero) {
    EXPECT_DOUBLE_EQ(mathlib::multiply(5.0, 0.0), 0.0);
}

TEST(MathMultiplyTest, NegativeNumbers) {
    EXPECT_DOUBLE_EQ(mathlib::multiply(-3.0, -4.0), 12.0);
}

TEST(MathMultiplyTest, MixedSigns) {
    EXPECT_DOUBLE_EQ(mathlib::multiply(-3.0, 4.0), -12.0);
}

TEST(MathDivideTest, PositiveNumbers) {
    EXPECT_DOUBLE_EQ(mathlib::divide(10.0, 2.0), 5.0);
}

TEST(MathDivideTest, FractionalResult) {
    EXPECT_DOUBLE_EQ(mathlib::divide(7.0, 2.0), 3.5);
}

TEST(MathDivideTest, NegativeNumbers) {
    EXPECT_DOUBLE_EQ(mathlib::divide(-10.0, -2.0), 5.0);
}

TEST(MathDivideTest, DivisionByZeroThrows) {
    EXPECT_THROW(mathlib::divide(5.0, 0.0), std::invalid_argument);
}
