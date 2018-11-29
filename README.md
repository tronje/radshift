# Radshift

[![Build Status](https://travis-ci.org/tronje/radshift.svg?branch=master)](https://travis-ci.org/tronje/radshift)

Radshift is a fork of Redshift, with most of its functionality ripped out.

## Why?

I believe that Redshift is good software, but much too complex and feature-rich
for what it actually does: set the screen temperature.
In particular, Redshift talks to GeoClue to determine location to then set the
screen temperature. Radshift simply looks at the system time.
Redshift provides different methods of changing the screen temperature, and
works on Linux, macOS, and Windows. Radshift provides only one method, and
only works on Linux.
Redshift is over 6000 lines of C code, Radshift less than 1000 lines of C code.

## Configuration

All configuration is optional. Defaults are already set, and radshift should
compile and work without any changes.
You may, however, want to:

* edit `include/config.h` to set your preferred times and temperatures.
  There's planned support for a config file and environment variables.
* edit `Makefile` to set the install-prefix (and any compilation options you want).
  You can also build with `make INSTALL_PREFIX=/path/to/radshift/binary`.
* edit `systemd/radshift.service` to your liking.

## Installation

These are the suggested steps to install radshift:

1. `make && sudo make install`
2. (optional) `cp systemd/radshift.service ~/.config/systemd/user/`
3. (optional) `systemctl --user enable radshift.service`
4. (optional) `systemctl --user start radshift.service`

You may also skip the `sudo make install` step, and simply set radshift's
location in the `.service` file.

## Usage

I recommend you use the Systemd User service. But you can also use Radshift
manually, e.g. `radshift --auto` to automatically set the screen temperature
to an appropriate value. Or `radshift 5000K` to set screen temperature to
5000 Kelvin.
You could also add `radshift --continuous &` to your `.xinitrc`, i3 config,
or similar startup/configuration file.
See `radshift --help` for more information.

## Attribution

Radshift's core functionality (setting screen temperature) was taken from
[Redshift](https://github.com/jonls/redshift). Radshift is thus also licensed
under GPL Version 3.

Jon Lund Steffensen is the copyright holder of Redshift, as well as the
following files contained in Radshift:
* `src/gamma-randr.c`
* `include/gamma-randr.h`
* `include/colorramp.h`

Jon Lund Steffensen and Ingo Thies are the copyright holders of the following
file contained in Radshift:
* `src/colorramp.c`

Ingo Thies is the copyright holder of the following file contained in Radshift:
* `README-colorramp`
