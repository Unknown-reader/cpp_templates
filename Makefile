CXX      = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -Wpedantic -Werror -g

all: demo

demo: demo.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

clean:
	rm -rf demo

.PHONY: all demo clean