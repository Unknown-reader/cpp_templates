CXX      = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -Wpedantic -Werror -g

BUILD := build
SRC   := demo.cpp

all: $(BUILD)/demo

$(BUILD)/demo: demo.cpp
	mkdir -p $(BUILD)
	$(CXX) $(CXXFLAGS) $< -o $@

clean:
	rm -rf $(BUILD)

.PHONY: all clean