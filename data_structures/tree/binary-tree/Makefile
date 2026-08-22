CPP := g++
CPPFLAGS := -std=c++17 -Wall -Wextra -g

all: tree

tree: main.cpp tree.hpp
	$(CPP) $(CPPFLAGS) main.cpp -o tree

run: tree
	./tree

clean:
	rm -f tree

.PHONY: all run clean
