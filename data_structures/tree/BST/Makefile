CPP := g++
CPPFLAGS := -std=c++17 -Wall -Wextra -g

all: bst

bst: main.cpp bst.hpp
	$(CPP) $(CPPFLAGS) main.cpp -o bst

run: bst
	./bst

clean:
	rm -f bst

.PHONY: all run clean
