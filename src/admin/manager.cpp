#include "admin/manager.hpp"
using namespace std;

EngineManager::EngineManager(int argc, const char* argv[]) {
    this->populate_ip_directory(argc, argv);
}

void EngineManager::execute() {}

pair<cid, cid> EngineManager::get_match() {}

void EngineManager::add_contribution(cid contribution_id) {}

void EngineManager::populate_ip_directory(int argc, const char* argv[]) {
    string last_ip_addr;

    // iteratively fill IP directory
    for (size_t i = 2; i < (size_t(argv[1]) + 1) * 2; i++) {
        if (i % 2) this->ip_directory[last_ip_addr] = uint32_t(argv[i]);
        else last_ip_addr = string(argv[i]);
    }
}