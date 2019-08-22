#include <cmath>
#include "core/scoring.hpp"
using namespace std;

std::pair<elo, elo> EngineScorer::update_scores(elo winner, elo loser) {
    return pair<elo, elo>(
        EngineScorer::calculate_rating(winner, loser, true),
        EngineScorer::calculate_rating(loser, winner, false)
    );
}

elo EngineScorer::calculate_rating(elo target, elo other, bool is_winner) {

    // calculate expected outcome
    const float exp = (other - target) / ELO_N_SCALE;
    const float expected_outcome = 1 / (1 + pow(ELO_EXP_BASE, exp));
    
    // calculate new rating
    return target + ELO_K_FACTOR * (is_winner - expected_outcome);
}