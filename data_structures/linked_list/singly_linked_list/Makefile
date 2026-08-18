CPP := g++
CPPFLAGS := -std=c++17 -Wall -Wextra -g

all: linked_list

linked_list: main.cpp linked_list.cpp linked_list.hpp
	$(CPP) $(CPPFLAGS) main.cpp linked_list.cpp -o linked_list

run: linked_list
	./linked_list

clean:
	rm -f linked_list

.PHONY: all run clean
