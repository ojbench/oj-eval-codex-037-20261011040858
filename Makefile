# Simple Makefile for ACMOJ submission
# Compiles main.cpp to an executable named `code`

CXX = g++
CXXFLAGS = -std=c++17 -O2 -pipe -static -s

all: code

code: main.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

.PHONY: clean all
clean:
	rm -f code
