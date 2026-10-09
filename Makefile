CXX      ?= g++
CXXFLAGS ?= -std=c++20 -Wall -Wextra -Wpedantic -Werror -g

BUILD   := build
LESSONS := $(wildcard lessons/*.cpp)
BINS    := $(patsubst lessons/%.cpp,$(BUILD)/%,$(LESSONS))

all: $(BINS)

$(BUILD)/%: lessons/%.cpp
	@mkdir -p $(BUILD)
	$(CXX) $(CXXFLAGS) $< -o $@

run: all
	@for bin in $(BINS); do \
		echo "=== $$bin ==="; \
		$$bin; \
	done

clean:
	rm -rf $(BUILD)

.PHONY: all run clean
