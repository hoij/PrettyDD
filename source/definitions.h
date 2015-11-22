#ifndef DEFINITIONS_H
#define DEFINITIONS_H

/* These are the main types that a log line is categorized
 * into. They are sorted roughly by (guessed) frequency of use. */
enum LineType {
    DAMAGE,
    NA,  // Not available
    HEAL,
    NANO_CAST,
    RESEARCH,
    SK,
    XP,
    AIXP,
    NANO,
    VP,
    PVP_TEAM_SCORE,
    PVP_SOLO_SCORE
};


#endif  // DEFINITIONS_H