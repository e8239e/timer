#ifndef PARSE_DURATION_HH_INCL
#define PARSE_DURATION_HH_INCL

#include <string>
extern "C" {
#include "parse_duration.h"
}

namespace duration
{

bool is_valid(const char *str)
{
	return parse_duration_is_valid_input(str);
}

bool is_valid(const std::string str)
{
	return is_valid(str.c_str());
}

std::int_least64_t parse(const char *str)
{
	return parse_duration(str);
}

std::int_least64_t parse(const std::string str)
{
	return parse(str.c_str());
}

}

#endif
