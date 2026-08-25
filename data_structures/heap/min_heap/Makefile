CPP := g++
CPPFLAGS := -std=c++17 -Wall -Wextra -g

all : min_heap

min_heap: main.cpp min_heap.hpp
	$(CPP) $(CPPFLAGS) main.cpp -o min_heap

run: min_heap
	./min_heap

clean:
	rm -f min_heap

.PHONY: all run clean
