#include "parse_duration.h"
#include <criterion/criterion.h>
#include <criterion/new/assert.h>

#define BUFSIZE 255
#define MAX 100
#define STEP 1

Test(input_check, must_fail)
{
	cr_expect(all(

		eq(parse_duration_valid_input(""), false),
		eq(parse_duration_valid_input("j"), false),
		eq(parse_duration_valid_input("s"), false),
		eq(parse_duration_valid_input("slevj"), false),
		eq(parse_duration_valid_input("dmh"), false),
		eq(parse_duration_valid_input("1hm"), false)

			));

	cr_expect(all(

		eq(i32, parse_duration(""), -1),
		eq(i32, parse_duration("j"), -1),
		eq(i32, parse_duration("s"), -1),
		eq(i32, parse_duration("slevj"), -1),
		eq(i32, parse_duration("dmh"), -1),
		eq(i32, parse_duration("1hm"), -1)

			));
}

Test(input_check, must_succeed)
{
	cr_expect(all(

		parse_duration_valid_input("0"),
		parse_duration_valid_input("0s"),
		parse_duration_valid_input("0m"),
		parse_duration_valid_input("0h"),
		parse_duration_valid_input("0d"),
		parse_duration_valid_input("1"),
		parse_duration_valid_input("13s"),
		parse_duration_valid_input("5d"),
		parse_duration_valid_input("1s5d3h100m")

			));

	cr_expect(all(

		eq(i32, parse_duration("0"), 0),
		eq(i32, parse_duration("0s"), 0),
		eq(i32, parse_duration("0m"), 0),
		eq(i32, parse_duration("0h"), 0),
		eq(i32, parse_duration("0d"), 0),
		eq(i32, parse_duration("1"), 1),
		eq(i32, parse_duration("13s"), 13),
		eq(i32, parse_duration("5d"), 5 * 3600 * 24),
		eq(i32, parse_duration("1s5d3h100m"),
		   1 + 5 * 3600 * 24 + 3 * 3600 + 100 * 60)

			));
}

Test(simple_input, plain)
{
	char num[BUFSIZE];
	int exp;
	for (int i = 0; i < MAX; i += STEP) {
		exp = i;
		sprintf(num, "%d", i);
		cr_expect(eq(i32, parse_duration(num), exp),
			  "parse_duration(%s) is expected to be %d", num, exp);
	}
}

Test(simple_input, with_seconds)
{
	char num[BUFSIZE];
	int exp;
	for (int i = 0; i < MAX; i += STEP) {
		exp = i;
		sprintf(num, "%ds", i);
		cr_expect(eq(i32, parse_duration(num), exp),
			  "parse_duration(%s) is expected to be %d", num, exp);
	}
}

Test(simple_input, with_minutes)
{
	char num[BUFSIZE];
	int exp;
	for (int i = 0; i < MAX; i += STEP) {
		exp = i * 60;
		sprintf(num, "%dm", i);
		cr_expect(eq(i32, parse_duration(num), exp),
			  "parse_duration(%s) is expected to be %d", num, exp);
	}
}

Test(simple_input, with_hours)
{
	char num[BUFSIZE];
	int exp;
	for (int i = 0; i < MAX; i += STEP) {
		exp = i * 60 * 60;
		sprintf(num, "%dh", i);
		cr_expect(eq(i32, parse_duration(num), exp),
			  "parse_duration(%s) is expected to be %d", num, exp);
	}
}

Test(complex_input, minutes_with_seconds_unnamed)
{
	char num[BUFSIZE];
	int exp;
	for (int i = 0; i < MAX; i += STEP) {
		for (int j = 0; j < MAX; j++) {
			exp = i * 60 + j;
			sprintf(num, "%dm%d", i, j);
			cr_expect(eq(i32, parse_duration(num), exp),
				  "parse_duration(%s) is expected to be %d",
				  num, exp);
		}
	}
}

Test(complex_input, minutes_with_seconds)
{
	char num[BUFSIZE];
	int exp;
	for (int i = 0; i < MAX; i += STEP) {
		for (int j = 0; j < MAX; j++) {
			exp = i * 60 + j;
			sprintf(num, "%dm%ds", i, j);
			cr_expect(eq(i32, parse_duration(num), exp),
				  "parse_duration(%s) is expected to be %d",
				  num, exp);
		}
	}
}

Test(complex_input, hours_with_minutes)
{
	char num[BUFSIZE];
	int exp;
	for (int i = 0; i < MAX; i += STEP) {
		for (int j = 0; j < MAX; j++) {
			exp = i * 60 * 60 + j * 60;
			sprintf(num, "%dh%dm", i, j);
			cr_expect(eq(i32, parse_duration(num), exp),
				  "parse_duration(%s) is expected to be %d",
				  num, exp);
		}
	}
}

Test(complex_input, hours_with_minutes_and_seconds_unnamed)
{
	char num[BUFSIZE];
	int exp;
	for (int i = 0; i < MAX; i += STEP) {
		for (int j = 0; j < MAX; j++) {
			for (int l = 0; l < MAX; l++) {
				exp = i * 60 * 60 + j * 60 + l;
				sprintf(num, "%dh%dm%d", i, j, l);
				cr_expect(
					eq(parse_duration(num), exp),
					"parse_duration(%s) is expected to be %d",
					num, exp);
			}
		}
	}
}

Test(complex_input, hours_with_minutes_and_seconds)
{
	char num[BUFSIZE];
	int exp;
	for (int i = 0; i < MAX; i += STEP) {
		for (int j = 0; j < MAX; j++) {
			for (int k = 0; k < MAX; k++) {
				exp = i * 60 * 60 + j * 60 + k;
				sprintf(num, "%dh%dm%ds", i, j, k);
				cr_expect(
					parse_duration(num) == exp,
					"parse_duration(%s) is expected to be %d",
					num, exp);
			}
		}
	}
}

Test(complex_input, days_with_hours_minutes_and_seconds)
{
	char num[BUFSIZE];
	int exp;
	for (int i = 0; i < MAX; i += STEP) {
		for (int j = 0; j < MAX; j++) {
			for (int k = 0; k < MAX; k++) {
				for (int l = 0; l < MAX; l++) {
					exp = (i * 60 * 60 * 24) +
					      (j * 60 * 60) + (k * 60) + l;
					sprintf(num, "%dd%dh%dm%ds", i, j, k,
						l);
					cr_expect(
						parse_duration(num) == exp,
						"parse_duration(%s) is expected to be %d",
						num, exp);
				}
			}
		}
	}
}

Test(complex_input, days_with_hours_minutes_and_seconds_unnamed)
{
	char num[BUFSIZE];
	int exp;
	for (int i = 0; i < MAX; i += STEP) {
		for (int j = 0; j < MAX; j++) {
			for (int k = 0; k < MAX; k++) {
				for (int l = 0; l < MAX; l++) {
					exp = (i * 60 * 60 * 24) +
					      (j * 60 * 60) + (k * 60) + l;
					sprintf(num, "%dd%dh%dm%d", i, j, k, l);
					cr_expect(
						parse_duration(num) == exp,
						"parse_duration(%s) is expected to be %d",
						num, exp);
				}
			}
		}
	}
}
