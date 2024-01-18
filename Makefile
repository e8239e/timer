LINK_FLAGS = -lINIReader -lsfml-audio -lsfml-system
COMMON_FLAGS = -flto=auto -O3 -pedantic -Wall -Werror -pie -pipe
CFLAGS ?= $(COMMON_FLAGS) -std=c99
CXXFLAGS ?= $(COMMON_FLAGS) -std=c++20
LDFLAGS = $(LINK_FLAGS)

build: timer

test:
	make test -C parse_duration

clean:
	rm -f *.o timer
	make clean -C parse_duration

fmt:
	clang-format -i $(shell find -type f -iname '*.[ch][ch]') $(shell find -type f -iname '*.[ch]')
