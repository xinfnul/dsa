CPP := g++
CPPFLAGS := -std=c++17 -Wall -Wextra -g

all: deque

deque: main.cpp deque.hpp
	$(CPP) $(CPPFLAGS) main.cpp -o deque

run: deque
	./deque

clean:
	rm -f deque

.PHONY: all run clean
