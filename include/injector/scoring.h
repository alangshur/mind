#ifndef SCORING_H
#define SCORING_H

#include <utility>
#include "orchestrator.h"

class EngineScorer {
    static std::pair<elo, elo> update_scores(elo winner, elo loser);
    static elo calculate_rating(elo target, elo other, bool is_winner);
};

#endif