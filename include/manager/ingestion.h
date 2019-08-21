#ifndef INGESTION_H
#define INGESTION_H

#include <queue>
#include <string>
#include <unistd.h>

/*
    The engine ingestor class monitors a piped file descriptor
    and reads in new-contribution and updated-contribution data, 
    which is then fed into two atomic queues for later handling
    by the rest of the engine pipeline.
*/
class EngineIngestor {
    public:
        void run_ingestion();
        EngineIngestor(int fd);
        std::queue<std::string> new_queue;
        std::queue<std::string> update_queue;
        
    private:
        int input_fd;
        ~EngineIngestor() {
            close(this->input_fd);
        }
};

#endif