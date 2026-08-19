CPP := g++
CPPFLAGS := -std=c++17 -Wall -Wextra -g

all: stack

stack: main.cpp stack.hpp
	$(CPP) $(CFLAGS) main.cpp -o stack

run: stack
	./stack

clean:
	rm -f stack

.PHONY: all run clean
