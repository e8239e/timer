#ifndef PARSE_DURATION_INCL
#define PARSE_DURATION_INCL
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

inline int_least32_t parse_duration__minutes(int x)
{
	return 60 * x;
}

inline int_least32_t parse_duration__hours(int x)
{
	return x * parse_duration__minutes(60);
}
inline int_least32_t parse_duration__days(int x)
{
	return x * parse_duration__hours(24);
}

bool parse_duration_valid_input(const char *str)
{
	bool not_empty = false;
	bool num_found = false;
	char c;
	while ((c = *str++)) {
		not_empty = true;
		switch (c) {
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
		case '0':
			num_found = true;
			break;
		case 's':
		case 'm':
		case 'h':
		case 'd':
			if (!num_found) {
				goto not_valid;
			}
			num_found = false;
			break;
		default:
			goto not_valid;
		}
		c = 0;
	}
	return not_empty;
not_valid:
	return false;
}

int_least32_t parse_duration(const char *str)
{
	if (!parse_duration_valid_input(str)) {
		return -1;
	}

#define __ADD(x)                                \
	ret += (x)*atoll(buf);                  \
	for (size_t ii = 0; ii < bufsize; ii++) \
		buf[ii] = '\0';

	int_least32_t ret = 0;
	size_t bufsize = strlen(str) + 1;
	char *buf = (char *)calloc(bufsize, sizeof(char));
	char c;
	while ((c = *str++)) {
		switch (c) {
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
		case '0':
			strncat(buf, &c, strlen(buf) + 1);
			break;
		case 's':
			__ADD(1);
			break;
		case 'm':
			__ADD(parse_duration__minutes(1));
			break;
		case 'h':
			__ADD(parse_duration__hours(1));
			break;
		case 'd':
			__ADD(parse_duration__days(1));
			break;
		default:
			return -1;
		}
		c = 0;
	}
	ret += atoll(buf);
	free(buf);
	return ret;
}
#endif
