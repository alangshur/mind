#include "admin/worker.hpp"
#include "admin/manager.hpp"
using namespace std;

/*
    argv:
        0 - "worker"/"manager" (string)
        1 - # of worker processes (uint32_t)
        n - worker {n / 2} IP (string)
        n + 1 - worker ((n - 1) / 2) tier (uint32_t)
*/
int main(int argc, const char* argv[]) {

    // launch worker process
    if (string(argv[0]) == "worker") {
        EngineWorker worker;
        worker.execute();
    }

    // launch manager process
    else if (string(argv[0]) == "manager") {
        EngineManager manager(argc, argv);
        manager.execute();
    }

    else return 0;
}