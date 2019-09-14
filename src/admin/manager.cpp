#include "admin/manager.hpp"
using namespace std;

EngineManager::EngineManager(int argc, const char* argv[]) {
    try {

        this->populate_file_descriptors(argv);
        this->populate_worker_directory(argc, argv);    
    }
    catch (...) {
        this->logger.log_error("EngineManager", "Error loading manager - "
            "shutting down prematurely.");
        this->trigger_manager_shutdown();
    }
}

void EngineManager::execute() {
    this->logger.log_message("EngineManager", "Launching engine manager.");

    // execute manager cycle
    try {
        while (true) {

            // read fd input header 
            char input_code;
            ssize_t bytes_read = read(this->read_fd, &input_code, 1);
            if (bytes_read == -1) goto manager_wind_down;

            // determine response
            write(this->write_fd, &input_code, 1);
            switch (input_code) {

                // handle shutdown
                case '0': goto manager_wind_down;
                
                // handle match request
                case '1': {
                    pair<cid, cid> match = this->get_match();
                    write(this->write_fd, &match.first, sizeof(match.first));
                    write(this->write_fd, &match.second, sizeof(match.second));
                    break;
                }

                // handle new contribution
                case '2': {
                    cid contribution_id;
                    read(this->read_fd, &contribution_id, sizeof(contribution_id));
                    this->add_contribution(contribution_id);
                    break;
                }
            }
        }
    }
    catch (exception& e) {
        this->logger.log_error("EngineManager", "Error executing manager: " 
            + string(e.what()));
    }

    manager_wind_down:;
    this->trigger_manager_shutdown();
    this->wait_manager_controllers();
    this->force_worker_shutdown();
}

pair<cid, cid> EngineManager::get_match() { 
    this->logger.log_message("EngineManager", "Fetching new match.");
    return pair<cid, cid>(0, 0); 
}

void EngineManager::add_contribution(cid contribution_id) {
    this->logger.log_message("EngineManager", "Adding new contribution with ID " 
        + to_string(contribution_id));
}

void EngineManager::trigger_manager_shutdown() {
    this->logger.log_message("EngineManager", "Shutting down manager.");
    close(this->read_fd);
    close(this->write_fd);
    this->global_shutdown_flag = true;
    this->global_shutdown_sem.post();
}

void EngineManager::wait_manager_controllers() {
    this->logger.log_message("EngineManager", "Closing manager controllers.");
}

void EngineManager::force_worker_shutdown() noexcept {
    this->logger.log_message("EngineManager", "Forcing workers to shutdown.");
}

void EngineManager::populate_file_descriptors(const char* argv[]) {
    this->read_fd = atoi(argv[3]);
    this->write_fd = atoi(argv[4]);
}

void EngineManager::populate_worker_directory(int argc, const char* argv[]) {
    string last_ip_addr;

    // iteratively fill IP directory
    size_t worker_count = size_t(atoi(argv[2]));
    for (size_t i = 5; i < (worker_count * 2) + 5; i++) {
        if (i % 2) last_ip_addr = string(argv[i]);
        else this->worker_directory[last_ip_addr] = { uint32_t(atoi(argv[i])), 0 };
    }
}