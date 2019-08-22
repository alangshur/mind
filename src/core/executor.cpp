#include "core/executor.hpp"

EngineExecutor::EngineExecutor(EngineIngestor& ingestor, EngineScorer& scorer, 
    EloStore& elo_store, ContributionStore& contribution_store) : ingestor(ingestor), 
    scorer(scorer), elo_store(elo_store), contribution_store(contribution_store) {}

