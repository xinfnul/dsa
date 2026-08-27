CPP := g++
CPPFLAGS := -std=c++17 -Wall -Wextra -g

all : hashmap

hashmap: main.cpp hashmap.hpp
	$(CPP) $(CPPFLAGS) main.cpp -o hashmap

run: hashmap
	./hashmap

clean:
	rm -f hashmap

.PHONY: all run clean
