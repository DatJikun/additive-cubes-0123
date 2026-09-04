CXX = g++
CXXFLAGS = -std=c++17 -O3 -march=native -Wall -Wextra -DNDEBUG

.PHONY: all test clean

all: bin/acf_tool bin/dump_deadends bin/run_candidate bin/taxonomy bin/votes bin/code_cassaigne bin/compare_alph bin/morph_forensics bin/desub_scan bin/quotient_scan

bin/acf_tool: src/acf_tool.cpp src/acf.hpp src/search.hpp
	mkdir -p bin
	$(CXX) $(CXXFLAGS) -o $@ src/acf_tool.cpp

bin/dump_deadends: src/dump_deadends.cpp src/acf.hpp src/search.hpp
	mkdir -p bin
	$(CXX) $(CXXFLAGS) -o $@ src/dump_deadends.cpp

bin/run_candidate: src/run_candidate.cpp src/acf.hpp src/search.hpp
	mkdir -p bin
	$(CXX) $(CXXFLAGS) -o $@ src/run_candidate.cpp

bin/taxonomy: src/taxonomy.cpp src/acf.hpp src/search.hpp
	mkdir -p bin
	$(CXX) $(CXXFLAGS) -o $@ src/taxonomy.cpp

bin/votes: src/votes.cpp src/acf.hpp src/search.hpp
	mkdir -p bin
	$(CXX) $(CXXFLAGS) -o $@ src/votes.cpp

bin/code_cassaigne: src/code_cassaigne.cpp src/acf.hpp src/search.hpp
	mkdir -p bin
	$(CXX) $(CXXFLAGS) -o $@ src/code_cassaigne.cpp

bin/compare_alph: src/compare_alph.cpp src/acf.hpp src/search.hpp
	mkdir -p bin
	$(CXX) $(CXXFLAGS) -o $@ src/compare_alph.cpp

bin/morph_forensics: src/morph_forensics.cpp src/acf.hpp src/search.hpp
	mkdir -p bin
	$(CXX) $(CXXFLAGS) -o $@ src/morph_forensics.cpp

bin/desub_scan: src/desub_scan.cpp src/acf.hpp src/search.hpp
	mkdir -p bin
	$(CXX) $(CXXFLAGS) -o $@ src/desub_scan.cpp

bin/quotient_scan: src/quotient_scan.cpp src/acf.hpp src/search.hpp
	mkdir -p bin
	$(CXX) $(CXXFLAGS) -o $@ src/quotient_scan.cpp

test: bin/acf_tool bin/desub_scan bin/quotient_scan
	./bin/acf_tool test
	python3 python/brute_verify.py
	python3 python/independent_checks.py
	python3 python/lattice_tau.py
	python3 python/desub_verify.py
	./bin/desub_scan famous
	./bin/quotient_scan famous
	python3 python/quotient_verify.py

clean:
	rm -f bin/*
