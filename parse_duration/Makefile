CXXFLAGS = -flto=auto -O3 -pedantic -Wall -Werror -pie -pipe -std=c++20
LDFLAGS = -lcriterion

test: parse_duration_test
	./$^

build: parse_duration_test

clean:
	rm -f *.o parse_duration_test_c parse_duration_test_cc
