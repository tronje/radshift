#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>

#include "config.h"
#include "gamma-randr.h"

static void print_help(FILE *io)
{
	fprintf(
		io,
		"Sets the temperature of the screen.\n"
		"Usage:\n"
		"  radshift --help         Print this help\n"
		"  radshift --auto         Automatically determine appropriate"
		" temperature and set it\n"
		"  radshift --continuous   Like --auto, but looped, executing"
		" like --auto every 30 minutes\n"
		"  radshift <temperature>  Set the screen temperature\n"
		"\n"
		"`temperature` should be between 2000 and 10000."
		" It is measured in Kelvin.\n"
		"A sensible night-time temperature is 4500K.\n"
		"A sensible daytime/neutral temperature is 6500K, which leaves"
		" the screen temperature unchanged.\n"
		"A higher temperature will increase the blue light, and a"
		" lower temperature the red.\n"
		"\n"
		"Radshift is a fork of Redshift. Redshift was written by"
		" John Lund Steffensen.\n"
		"Radshift and Redshift are licensed under the GNU General"
		" Public License, version 3.\n"
		"Redshift copyright is held by John Lund Steffensen.\n"
	);
}

static int set_temperature(int temperature)
{
	int rc;
	randr_state_t *state;
	rc = randr_init(&state);
	if (rc != 0) {
		fprintf(stderr, "Failed to init randr!\n");
		return rc;
	}

	rc = randr_start(state);
	if (rc != 0) {
		fprintf(stderr, "Failed to start randr!\n");
		return rc;
	}

	color_setting_t cs = {
		.temperature = temperature,
		.gamma = {1.0, 1.0, 1.0},
		.brightness = 1.0
	};

	rc = randr_set_temperature(state, &cs, 0);
	if (rc != 0) {
		fprintf(stderr, "Failed to set temperature!\n");
	} else {
		fprintf(stderr, "Set screen temperature to %dK.\n",
				temperature);
	}

	randr_free(state);

	return rc;
}

static int auto_set()
{
	time_t seconds_since_epoch = time(NULL);
	struct tm *local_time = localtime(&seconds_since_epoch);
	int hour = local_time->tm_hour;
	int temperature = NEUTRAL_TEMPERATURE;

	if (hour >= EVENING_START && hour < NIGHT_START) {
		temperature = EVENING_TEMPERATURE;
	} else if (hour >= NIGHT_START || hour < DAY_START) {
		temperature = NIGHT_TEMPERATURE;
	}

	int rc = set_temperature(temperature);

	return rc;
}

static int run_continuously()
{
	int rc;

	while (true) {
		rc = auto_set();
		if (rc != 0) {
			fprintf(stderr, "set_temperature() failed!\n");
			fprintf(stderr, "Will try again in %d seconds.\n",
					CONTINUOUS_INTERVAL);
		}

		rc = sleep(CONTINUOUS_INTERVAL);
		if (rc != 0) {
			fprintf(stderr, "sleep() failed! Exiting...");
			return rc;
		}
	}

	return 0;
}

int main(int argc, char **argv)
{
	if (argc != 2) {
		print_help(stderr);
		return EXIT_FAILURE;
	}

	if (strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0) {
		print_help(stdout);
		return EXIT_SUCCESS;
	}

	int rc;

#ifdef __OpenBSD__
	if (pledge("stdio rpath unix", NULL) == -1) {
		fprintf(stderr, "pledge\n");
		return EXIT_FAILURE;
	}
#endif /* __OpenBSD__ */

	if (strcmp(argv[1], "--auto") == 0) {
		rc = auto_set();
	} else if (strcmp(argv[1], "--continuous") == 0) {
		rc = run_continuously();
	} else {
		int temperature = atoi(argv[1]);
		if (temperature < 2000 || temperature >= 10000) {
			fprintf(stderr, "Using absurd temperature, aborting!\n");
			return EXIT_FAILURE;
		}
		rc = set_temperature(temperature);
	}

	return rc;
}
