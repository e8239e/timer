LINK_FLAGS = -lINIReader -lsfml-audio -lsfml-system
CFLAGS ?= -flto -O3 -std=c++20 -pedantic -Wall -Werror

build: timer.cpp
	$(CXX) $(CFLAGS) $(LINK_FLAGS) timer.cpp -o timer

test:
	make test -C parse_duration

clean:
	rm -f *.o timer

fmt:
	clang-format -i $(shell find -type f -iname '*.[ch]pp') $(shell find -type f -iname '*.[ch]')
