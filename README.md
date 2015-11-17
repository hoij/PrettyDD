# A Damage Logger for Anarchy Online

## Features
#### Damage Logging
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

#### Nano Program Logging
Show a list of all nano programs executed by you and the result of executing them. Available stats are:
 * Lands
 * Resists
 * Aborts
 * Counters
 * Fumbles
 * Full NCUs

#### XP Logging
Show XP per hour, max and min. Either gained or lost. Logs the following types:
* XP
* AIXP
* SK
* Research
* Solo PVP Score
* Team PVP Score


#### General
Control the program via in-game chat or a console window:
* Determine which stat view to show
* Pause and resume logging to stop logging damage and stop the DPM/XP timer
* Reset all data parsed (this will not clear the log file)
* Display the results of a command by executing a single script (/pdd). Too see all commands available type /pddhelp.

Commands entered into the console window can ommit the "pdd" part.

Chose to parse the log form the beginning or the end by changing a paramater in the settings file.

## Information about the statistics

####DPM
* DPM is individual per player and calculated from the time of the first action (attack, heal, nano cast, etc.) to the time of the last action.

####Damage per Opponent and Detailed Top List ([Player], dtop)
* Regular, special, nanobot and shield percentage is calculated as percentage of total damage
* Crit, deflect and miss percentages are CatHits*

####Damage per Type (types [Player1] [Player2])
* Regular and nanobot percentages are calculated as percentage of total damage
* Crit, deflect and miss percentages are CatHits*
* A stat that isn't possible to log or doesn't exist is shown as a blank line
* Misses from regular attacks have no type info and can't be logged per type. Regular misses are instead shown as a separate type.

####Detailed Damage per Type (dtypes [Player1] [Player2])
* "Regular Total" is the sum of normal, crit and deflect damage
* "Dmg%" is calculated as percentage of total damage
* "TotHit%" (Total hit percentage) is calculated as percentage of all hits for that type
* "CatHit%" see *

\*CatHits (Category hit percentage)
* This hit rate is calculated as percentage of all hits that can crit, deflect, miss and hit normally

####XP
* Min xp gained ignores xp of value 1 (don't worry, it's still included in the total)


## Install Instructions
See [INSTALL.md](INSTALL.md)
