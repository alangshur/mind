#ifndef MANAGER_H
#define MANAGER_H

#include <map>
#include "util/logger.hpp"
#include "core/elo-store.hpp"
#include "core/contribution-store.hpp"
#include "exec/ingestion-executor.hpp"
#include "exec/match-executor.hpp"
#include "exec/control-executor.hpp"
#include "portal/ingestion-portal.hpp"
#include "portal/match-portal.hpp"
#include "portal/control-portal.hpp"
#include "admin/definitions.hpp"

/*
    The EngineWorker class is the central 
    management unit for the manager process. It manages
    the massive amount of communication between all the 
    worker processes to rank contributions and generate 
    new match-ups.
*/
class EngineManager {
    public:
        EngineManager(int argc, const char* argv[]);
        void execute();
        std::pair<cid, cid> get_match();
        void add_contribution(cid contribution_id);

    private:
        void populate_ip_directory(int argc, const char* argv[]);
        std::map<std::string, uint32_t> ip_directory;
};

#endif

