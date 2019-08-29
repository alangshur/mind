#ifndef INGESTION_PORTAL_H
#define INGESTION_PORTAL_H

#include "util/tcp-server.hpp"
#include "exec/ingestion-executor.hpp"

class EngineIngestionPortal {
    public:
        EngineIngestionPortal(EngineIngestionExecutor& executor);
};

#endif