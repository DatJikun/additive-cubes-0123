CXX = g++
CXXFLAGS = -std=c++17 -O3 -march=native -Wall -Wextra -DNDEBUG

.PHONY: all test clean

all: bin/acf_tool bin/dump_deadends bin/run_candidate

bin/acf_tool: src/acf_tool.cpp src/acf.hpp src/search.hpp
	mkdir -p bin
	$(CXX) $(CXXFLAGS) -o $@ src/acf_tool.cpp

bin/dump_deadends: src/dump_deadends.cpp src/acf.hpp src/search.hpp
	mkdir -p bin
	$(CXX) $(CXXFLAGS) -o $@ src/dump_deadends.cpp

bin/run_candidate: src/run_candidate.cpp src/acf.hpp src/search.hpp
	mkdir -p bin
	$(CXX) $(CXXFLAGS) -o $@ src/run_candidate.cpp

test: bin/acf_tool
	./bin/acf_tool test
	python3 python/brute_verify.py

clean:
	rm -f bin/acf_tool
