#ifndef DEFINITIONS_H
#define DEFINITIONS_H

/* These are the main types that a log line is categorized
 * into. They are sorted roughly by (guessed) frequency of use. */
enum class LineType {
    damage,
    notAvailable,
    heal,
    nanoCast,
    sk,
    research,
    xp,
    aixp,
    nano,
    vp,
    pvpTeamScore,
    pvpSoloScore
};


#endif  // DEFINITIONS_H