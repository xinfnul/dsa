CPP := g++
CPPFLAGS := -std=c++17 -Wall -Wextra -g

all : max_heap

max_heap: main.cpp max_heap.hpp
	$(CPP) $(CPPFLAGS) main.cpp -o max_heap

run: max_heap
	./max_heap

clean:
	rm -f max_heap

.PHONY: all run clean
