# A Damage Logger for Anarchy Online

## Features
### Damage Logging
Logs all damage dealt and received and has the ability to display the following stats:
* Total damage and damage per minute
* Distribution of damage between regular, special, nanobot and shield damage
* Miss percentage for regular and special attacks
* Crit and deflect percentage
* Max and min hits per type

The stats can be displayed in several different ways:
 * As overall top lists
 * For a specific player. Either per opponent or per type
 * For a specific opponent of a specific player per type

### Nano Program Logging
Show a list of all nano programs executed by you and the result of executing them. Available stats are:
 * Lands
 * Resists
 * Aborts
 * Counters
 * Fumbles
 * Full NCUs

### XP Logging
Show XP per hour, max and min. Either gained or lost. Logs the following types:
* XP
* AIXP
* SK
* Research
* Solo PVP Score
* Team PVP Score


### General
Control the program via in-game chat:
* Determine which stat view to show
* Pause and resume logging to stop logging damage and stop the DPM/XP timer
* Reset all data parsed (this will not clear the log file)
* Display the results of a command by executing a single script (/pdd). Too see all commands available type "pdd help" in game

Chose to parse the log form the beginning or the end by changing a setting parameter.

### Info
Damage per minute is individual per player and calculated form the time that a player first performs any action (e.g. hit or cast a nano) which adds it to the program.

## Install Instructions
See [INSTALL_INSTRUCTIONS.md](INSTALL_INSTRUCTIONS.md)
