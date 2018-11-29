#ifndef CONFIG_H
#define CONFIG_H

/*** edit to your liking ***/
/* temperature used during daytime */
#define NEUTRAL_TEMPERATURE 6500

/* temperature used in the evening */
#define EVENING_TEMPERATURE 4500

/* temperature used during the night */
#define NIGHT_TEMPERATURE   3500

/* when does your day/evening/night start? */
#define DAY_START      7
#define EVENING_START 20
#define NIGHT_START   22

/* interval at which temperature is updated in continuous mode */
#define CONTINUOUS_INTERVAL 1800 /* seconds */

#endif /* CONFIG_H */
