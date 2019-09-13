#ifndef MANAGER_H
#define MANAGER_H

#include <mutex>
#include <queue>
#include <utility>
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

typedef struct {
    uint32_t tier;
    uint32_t contribution_count;
} worker_t;

/*
    The EngineWorker class is the central 
    management unit for the manager process. It manages
    the massive amount of communication between all the 
    worker processes to rank contributions and generate 
    new match-ups.

    I/O codes:
        - 0: shutdown
        - 1: match
        - 2: contribution

    Input structure:
        - Header:
            - 1 byte: input code 
        - Payload:
            - shutdown: NULL (0 bytes)
            - match: NULL (0 bytes)
            - contribution: cid (4 bytes)

    Output structure:
        - Header:
            - 1 byte: output code 
        - Payload:
            - shutdown: NULL (0 bytes)
            - match: cid 1, cid 2 (8 bytes)
            - contribution: NULL (0 bytes)
*/
class EngineManager : private ShutdownThread {
    public:
        EngineManager(int argc, const char* argv[]);
        void execute();

    private:
        std::pair<cid, cid> get_match();
        void add_contribution(cid contribution_id);
        void trigger_manager_shutdown();
        void wait_manager_controllers();
        void force_worker_shutdown() noexcept;
        void populate_file_descriptors(const char* argv[]);
        void populate_worker_directory(int argc, const char* argv[]);

        std::mutex match_queue_mutex;
        std::queue<std::pair<cid, cid>> match_queue;
        std::map<std::string, worker_t> worker_directory;
        Logger logger;
        uint32_t read_fd;
        uint32_t write_fd;
};

#endif

