#ifndef ELO_SCORING_H
#define ELO_SCORING_H

#include <utility>
#include "orchestrator.hpp"

/*
    The engine scorer class contains two static methods that are
    used to quickly calculate updated ELO scores given a result 
    involving two previous ELO scores.
*/
class EngineScorer {
    public:
        static std::pair<elo, elo> update_scores(elo winner, elo loser);
        static elo calculate_rating(elo target, elo other, bool is_winner);
};

#endif