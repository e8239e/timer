#include <chrono>

#include <criterion/criterion.h>
#include <criterion/new/assert.h>

#include "parse_duration.hh"

#define BUFSIZE 255
#define MAX 100
#define STEP 1

Test(input_check, must_fail)
{
	cr_expect(all(

		eq(duration::is_valid(""), false),
		eq(duration::is_valid("j"), false),
		eq(duration::is_valid("s"), false),
		eq(duration::is_valid("slevj"), false),
		eq(duration::is_valid("dmh"), false),
		eq(duration::is_valid("1hm"), false)

			));
}

Test(input_check, must_succeed)
{
	cr_expect(all(

		duration::is_valid("0"), duration::is_valid("0s"),
		duration::is_valid("0m"), duration::is_valid("0h"),
		duration::is_valid("0d"), duration::is_valid("1"),
		duration::is_valid("13s"), duration::is_valid("5d"),
		duration::is_valid("1s5d3h100m")

			));

	cr_expect(all(

		eq(duration::parse("0"), std::chrono::seconds(0).count()),
		eq(duration::parse("0s"), std::chrono::seconds(0).count()),
		eq(duration::parse("0m"), std::chrono::seconds(0).count()),
		eq(duration::parse("0h"), std::chrono::seconds(0).count()),
		eq(duration::parse("0d"), std::chrono::seconds(0).count()),
		eq(duration::parse("1"), std::chrono::seconds(1).count()),
		eq(duration::parse("13s"), std::chrono::seconds(13).count()),
		eq(duration::parse("5d"), std::chrono::seconds(std::chrono::hours(5 * 24)).count()),
		eq(duration::parse("1s5d3h100m"),
		   (std::chrono::seconds(1) + std::chrono::hours(5 * 24) +
		    std::chrono::hours(3) + std::chrono::minutes(100))
			   .count())

			));
}

Test(simple_input, plain)
{
	char num[BUFSIZE];
	std::chrono::seconds exp;
	for (int i = 0; i < MAX; i += STEP) {
		exp = std::chrono::seconds(i);
		snprintf(num, BUFSIZE, "%d", i);
		cr_expect(eq(duration::parse(num), exp.count()),
			  "duration::parse_chrono(%s) is expected to be %d",
			  num, exp.count());
	}
}

Test(simple_input, with_seconds)
{
	char num[BUFSIZE];
	std::chrono::seconds exp;
	for (int i = 0; i < MAX; i += STEP) {
		exp = std::chrono::seconds(i);
		snprintf(num, BUFSIZE, "%ds", i);
		cr_expect(eq(duration::parse(num), exp.count()),
			  "duration::parse_chrono(%s) is expected to be %d",
			  num, exp.count());
	}
}

Test(simple_input, with_minutes)
{
	char num[BUFSIZE];
	std::chrono::seconds exp;
	for (int i = 0; i < MAX; i += STEP) {
		exp = std::chrono::minutes(i);
		snprintf(num, BUFSIZE, "%dm", i);
		cr_expect(eq(duration::parse(num), exp.count()),
			  "duration::parse_chrono(%s) is expected to be %d",
			  num, exp.count());
	}
}

Test(simple_input, with_hours)
{
	char num[BUFSIZE];
	std::chrono::seconds exp;
	for (int i = 0; i < MAX; i += STEP) {
		exp = std::chrono::hours(i);
		snprintf(num, BUFSIZE, "%dh", i);
		cr_expect(eq(duration::parse(num), exp.count()),
			  "duration::parse_chrono(%s) is expected to be %d",
			  num, exp.count());
	}
}

Test(complex_input, minutes_with_seconds_unnamed)
{
	char num[BUFSIZE];
	std::chrono::seconds exp;
	for (int i = 0; i < MAX; i += STEP) {
		for (int j = 0; j < MAX; j++) {
			exp = std::chrono::minutes(i) + std::chrono::seconds(j);
			snprintf(num, BUFSIZE, "%dm%d", i, j);
			cr_expect(
				eq(duration::parse(num), exp.count()),
				"duration::parse_chrono(%s) is expected to be %d",
				num, exp.count());
		}
	}
}

Test(complex_input, minutes_with_seconds)
{
	char num[BUFSIZE];
	std::chrono::seconds exp;
	for (int i = 0; i < MAX; i += STEP) {
		for (int j = 0; j < MAX; j++) {
			exp = std::chrono::minutes(i) + std::chrono::seconds(j);
			snprintf(num, BUFSIZE, "%dm%ds", i, j);
			cr_expect(
				eq(duration::parse(num), exp.count()),
				"duration::parse_chrono(%s) is expected to be %d",
				num, exp.count());
		}
	}
}

Test(complex_input, hours_with_minutes)
{
	char num[BUFSIZE];
	std::chrono::seconds exp;
	for (int i = 0; i < MAX; i += STEP) {
		for (int j = 0; j < MAX; j++) {
			exp = std::chrono::hours(i) + std::chrono::minutes(j);
			snprintf(num, BUFSIZE, "%dh%dm", i, j);
			cr_expect(
				eq(duration::parse(num), exp.count()),
				"duration::parse_chrono(%s) is expected to be %d",
				num, exp.count());
		}
	}
}

Test(complex_input, hours_with_minutes_and_seconds_unnamed)
{
	char num[BUFSIZE];
	std::chrono::seconds exp;
	for (int i = 0; i < MAX; i += STEP) {
		for (int j = 0; j < MAX; j++) {
			for (int l = 0; l < MAX; l++) {
				exp = std::chrono::hours(i) +
				      std::chrono::minutes(j) +
				      std::chrono::seconds(l);
				snprintf(num, BUFSIZE, "%dh%dm%d", i, j, l);
				cr_expect(
					eq(duration::parse(num), exp.count()),
					"duration::parse_chrono(%s) is expected to be %d",
					num, exp.count());
			}
		}
	}
}

Test(complex_input, hours_with_minutes_and_seconds)
{
	char num[BUFSIZE];
	std::chrono::seconds exp;
	for (int i = 0; i < MAX; i += STEP) {
		for (int j = 0; j < MAX; j++) {
			for (int l = 0; l < MAX; l++) {
				exp = std::chrono::hours(i) +
				      std::chrono::minutes(j) +
				      std::chrono::seconds(l);
				snprintf(num, BUFSIZE, "%dh%dm%ds", i, j, l);
				cr_expect(
					eq(duration::parse(num), exp.count()),
					"duration::parse_chrono(%s) is expected to be %d",
					num, exp.count());
			}
		}
	}
}

Test(complex_input, days_with_hours_minutes_and_seconds)
{
	char num[BUFSIZE];
	std::chrono::seconds exp;
	for (int i = 0; i < MAX; i += STEP) {
		for (int j = 0; j < MAX; j++) {
			for (int k = 0; k < MAX; k++) {
				for (int l = 0; l < MAX; l++) {
					exp = std::chrono::hours(24 * i) +
					      std::chrono::hours(j) +
					      std::chrono::minutes(k) +
					      std::chrono::seconds(l);
					snprintf(num, BUFSIZE, "%dd%dh%dm%ds",
						 i, j, k, l);
					cr_expect(
						eq(duration::parse(num),
						   exp.count()),
						"duration::parse_chrono(%s) is expected to be %d",
						num, exp.count());
				}
			}
		}
	}
}

Test(complex_input, days_with_hours_minutes_and_seconds_unnamed)
{
	char num[BUFSIZE];
	std::chrono::seconds exp;
	for (int i = 0; i < MAX; i += STEP) {
		for (int j = 0; j < MAX; j++) {
			for (int k = 0; k < MAX; k++) {
				for (int l = 0; l < MAX; l++) {
					exp = std::chrono::hours(24 * i) +
					      std::chrono::hours(j) +
					      std::chrono::minutes(k) +
					      std::chrono::seconds(l);
					snprintf(num, BUFSIZE, "%dd%dh%dm%d", i,
						 j, k, l);
					cr_expect(
						eq(duration::parse(num),
						   exp.count()),
						"duration::parse_chrono(%s) is expected to be %d",
						num, exp.count());
				}
			}
		}
	}
}
