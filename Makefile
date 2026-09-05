CXX = g++
CXXFLAGS = -std=c++17 -O3 -march=native -Wall -Wextra -DNDEBUG

.PHONY: all test clean

all: bin/acf_tool bin/dump_deadends bin/run_candidate bin/taxonomy bin/votes bin/code_cassaigne bin/compare_alph bin/morph_forensics bin/desub_scan bin/quotient_scan bin/triple_scan bin/template_scan bin/kernel_scan bin/pt_scan bin/pt_verify bin/core_scan

bin/core_scan: src/core_scan.cpp src/acf.hpp src/search.hpp
	mkdir -p bin
	$(CXX) $(CXXFLAGS) -o $@ src/core_scan.cpp

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

bin/triple_scan: src/triple_scan.cpp src/acf.hpp src/search.hpp
	mkdir -p bin
	$(CXX) $(CXXFLAGS) -o $@ src/triple_scan.cpp

bin/template_scan: src/template_scan.cpp src/acf.hpp src/search.hpp
	mkdir -p bin
	$(CXX) $(CXXFLAGS) -o $@ src/template_scan.cpp

bin/kernel_scan: src/kernel_scan.cpp src/acf.hpp src/search.hpp
	mkdir -p bin
	$(CXX) $(CXXFLAGS) -o $@ src/kernel_scan.cpp

bin/pt_scan: src/pt_scan.cpp src/acf.hpp
	mkdir -p bin
	$(CXX) $(CXXFLAGS) -o $@ src/pt_scan.cpp

bin/pt_verify: src/pt_verify.cpp src/acf.hpp
	mkdir -p bin
	$(CXX) $(CXXFLAGS) -o $@ src/pt_verify.cpp

test: bin/acf_tool bin/desub_scan bin/quotient_scan bin/triple_scan bin/template_scan bin/kernel_scan bin/pt_verify bin/core_scan
	./bin/acf_tool test
	python3 python/brute_verify.py
	python3 python/independent_checks.py
	python3 python/lattice_tau.py
	python3 python/desub_verify.py
	./bin/desub_scan famous
	./bin/quotient_scan famous
	python3 python/quotient_verify.py
	./bin/triple_scan famous
	python3 python/triple_verify.py
	./bin/template_scan
	python3 python/template_verify.py
	./bin/kernel_scan | tee data/kernel_cert_cpp.txt
	python3 python/kernel_regular.py
	python3 python/kernel_verify.py
	python3 python/pt_acf.py
	./bin/pt_verify | tee data/pt_verify_cpp.txt
	python3 python/pt_verify.py
	./bin/core_scan detfsm
	./bin/core_scan cycle 6
	./bin/core_scan inject 5
	./bin/core_scan drive 6 tm 100 blind
	./bin/core_scan tmblocks 3 3 50 0
	./bin/core_scan beam 16 64 0
	./bin/core_scan inject_iter 3 2 1
	python3 python/core_verify.py

clean:
	rm -f bin/*
