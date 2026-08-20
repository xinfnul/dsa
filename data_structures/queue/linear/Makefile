CPP := g++
CPPFLAGS := -std=c++17 -Wall -Wextra -g

all: queue

queue: main.cpp queue.hpp
	$(CPP) $(CPPFLAGS) main.cpp -o queue

run: queue
	./queue

clean:
	rm -f queue

.PHONY: all run clean
