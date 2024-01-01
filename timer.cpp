#include <cstdio>
#include <cstdint>
#include <iostream>
#include <filesystem>
#include <format>

#ifdef _WIN32
#include <Windows.h>
#define SLEEP_SEC(x) Sleep(x * 1000)
#else
#include <unistd.h>
#define SLEEP_SEC(x) sleep(x)
#endif

extern "C" {
#include "parse_duration/parse_duration.h"
}

#include <INIReader.h>
#include <SFML/Audio.hpp>

#define die(x, y)                               \
	std::cerr << "ERR: " << x << std::endl; \
	std::exit(y)

namespace fs = std::filesystem;

std::string remaining(const int_least32_t dur)
{
	auto tmp = dur;
	auto r_hours = tmp / 3600;
	tmp -= parse_duration__hours(tmp / 3600);
	auto r_minutes = tmp / 60;
	tmp -= parse_duration__minutes(tmp / 60);
	auto r_seconds = tmp;
	return std::format("{:0>2}h {:0>2}m {:0>2}s", r_hours, r_minutes,
			   r_seconds);
}

int main(int argc, char **argv)
{
	if (argc < 2) {
		die("Duration not provided"
			    << std::endl
			    << std::endl
			    << "Usage: " << argv[0] << " DURATION" << std::endl
			    << "Arguments:" << std::endl
			    << "\tDURATION - string with format like 1d2h3m4s which means 1 day 2 hours 3 minutes 4 seconds",
		    1);
	} else if (!parse_duration_valid_input(argv[1])) {
		die("Duration is not valid", 2);
	}

	auto xdg_config_dir_raw = std::getenv("XDG_CONFIG_HOME");
	std::string xdg_config_dir;
	if (xdg_config_dir_raw == nullptr) {
		auto home = std::getenv("HOME");
		if (home == nullptr) {
			die("$XDG_CONFIG_HOME and $HOME not set, can't find config location",
			    3);
		} else {
			std::string tmp =
				std::string(home) + std::string("/.config");
			xdg_config_dir = tmp;
		}
	}
	const std::string config_dir = xdg_config_dir + "/timer";
	const auto config_file = config_dir + "/config.ini";

	INIReader reader(config_file);

	if (reader.ParseError() < 0) {
		std::fprintf(
			stderr,
			"WARN: Can't load config file at '%s', trying to create new\n",
			config_file.c_str());
		if (!fs::exists(fs::path(config_dir))) {
			try {
				fs::create_directories(config_dir);
			} catch (...) {
				die("Can't create config directory '"
					    << config_dir << "'",
				    4);
			}
		}
		FILE *f = std::fopen(config_file.c_str(), "w");
		if (f == nullptr) {
			die("Can't load config file at '"
				    << config_file << "', trying to create new",
			    5);
		}
		std::fprintf(f,
			     "sound_path=/path/to/sound/file.[ogg|flac|wav]\n");
		std::fclose(f);
		die("Default configuration written in '"
			    << config_file << "', check and adapt",
		    6);
	}
	const auto sound_path = reader.GetString("", "sound_path", "");

	sf::Music sound;
	if (!sound.openFromFile(sound_path)) {
		die("Failed to open sound", 7);
	}
	sound.setLoop(true);

	const int_fast64_t duration = parse_duration(argv[1]);
	int_fast64_t remain = duration;

	while (remain > -1) {
		std::fflush(stderr);
		std::cerr << remaining(remain--);
		std::fflush(stderr);
		if (remain >= 0) {
			SLEEP_SEC(1);
		}
		std::cerr << "\r";
		std::fflush(stderr);
	}
	std::cerr << std::endl;
	std::fflush(stderr);

	sound.play();

	std::cout << "Press Enter to exit ";
	std::getchar();

	sound.stop();
	return 0;
}
