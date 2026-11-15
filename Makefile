BASE_SRC := openlesson-base/src
NEXT_SRC := openlesson-next/src

ifeq ($(OS),Windows_NT)
    CXX := g++
    EXE := .exe
else ifeq ($(shell uname -s),Darwin)
    CXX := clang++
else
    CXX := g++
endif

CXXFLAGS ?= -std=c++17 -Wall -Wextra -g
QT_CMAKE ?= qt-cmake


.PHONY: base-build next-build clean

base-build:
	mkdir -p outputs/OLB
	"$(QT_CMAKE)" -S . -B outputs/OLB
	cmake --build outputs/OLB

next-build:
	mkdir -p outputs/NEXT
	$(CXX) $(CXXFLAGS) $$(find "$(NEXT_SRC)" -name '*.cpp') -o "outputs/NEXT/openlesson-next$(EXE)"
	
clean:
	rm -fr ./outputs
