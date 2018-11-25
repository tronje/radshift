#ifndef RADSHIFT_H
#define RADSHIFT_H

/* edit to your liking */
#define NEUTRAL_TEMPERATURE 6500
#define EVENING_TEMPERATURE 4500
#define NIGHT_TEMPERATURE   3500
#define CONTINUOUS_INTERVAL 1800 /* seconds */

/* Color setting */
typedef struct {
	int temperature;
	float gamma[3];
	float brightness;
} color_setting_t;

#endif // RADSHIFT_H
