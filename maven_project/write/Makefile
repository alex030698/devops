CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -I src/main/include
BUILD_DIR = target
SRC = src/main/c++/write.cpp
OBJ = $(BUILD_DIR)/write.o
LIB = $(BUILD_DIR)/libwrite.a
TEST_SRC = src/test/c++/write_test.cpp
TEST_BIN = $(BUILD_DIR)/write_test

.PHONY: all clean test

all: $(LIB)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(OBJ): $(SRC) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(LIB): $(OBJ)
	ar rcs $@ $^

$(TEST_BIN): $(TEST_SRC) $(LIB) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $< -L$(BUILD_DIR) -lwrite -lgtest -lgtest_main -pthread -o $@

test: $(TEST_BIN)
	./$(TEST_BIN)

clean:
	rm -rf $(BUILD_DIR)
