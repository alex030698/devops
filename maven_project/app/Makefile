CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -I ../math/src/main/include -I ../read/src/main/include -I ../write/src/main/include
BUILD_DIR = target
LIBS_DIR_MATH = ../math/target
LIBS_DIR_READ = ../read/target
LIBS_DIR_WRITE = ../write/target
SRC = src/main/c++/main.cpp
BIN = $(BUILD_DIR)/app
TEST_SRC = src/test/c++/app_test.cpp
TEST_BIN = $(BUILD_DIR)/app_test

.PHONY: all clean test

all: $(BIN)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BIN): $(SRC) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $< -L$(LIBS_DIR_MATH) -L$(LIBS_DIR_READ) -L$(LIBS_DIR_WRITE) -lmath -lread -lwrite -o $@

$(TEST_BIN): $(TEST_SRC) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $< -L$(LIBS_DIR_MATH) -L$(LIBS_DIR_READ) -L$(LIBS_DIR_WRITE) -lmath -lread -lwrite -lgtest -lgtest_main -pthread -o $@

test: $(TEST_BIN)
	./$(TEST_BIN)

clean:
	rm -rf $(BUILD_DIR)
