#ifndef INGESTION_H
#define INGESTION_H

#include <queue>
#include <string>
#include <atomic>
#include <utility>
#include <unistd.h>
#include "orchestrator.hpp"
#include "manager/semaphore.hpp"

/*
    The engine ingestor class monitors a piped file descriptor
    and reads in new-contribution and updated-contribution data, 
    which is then fed into two atomic queues for later handling
    by the rest of the engine pipeline.
*/
class EngineIngestor {
    public:
        EngineIngestor(int read_fd, int write_fd);
        ~EngineIngestor();
        void run_ingestion();
        void shutdown_ingestor();

        std::atomic<std::queue<cid>*> new_queue;
        std::atomic<std::queue<std::pair<cid, cid>>*> update_queue;
        EffSemaphore new_queue_sem;
        EffSemaphore update_queue_sem;
        
    private:
        std::pair<cid, cid> read_fd_packet();

        int input_fd;
        int self_input_fd;
        EffSemaphore binary_shutdown_sem;
};

#endif