LINK_FLAGS = -lINIReader -lsfml-audio -lsfml-system
CFLAGS ?= -flto -O3 -std=c++20 -pedantic -Wall -Werror -pie -pipe
CXXFLAGS ?= $(CFLAGS)
LDFLAGS = $(LINK_FLAGS)

build: timer

test:
	make test -C parse_duration

clean:
	rm -f *.o timer

fmt:
	clang-format -i $(shell find -type f -iname '*.[ch][ch]') $(shell find -type f -iname '*.[ch]')
