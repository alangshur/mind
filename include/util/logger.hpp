#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <ctime>
#include <string>

/*
    The logger class is a simple logging utility to print
    formatted date-time messages and errors.
*/
typedef uint8_t node;
class Logger {
    public:
        Logger(node node_num) : node_num(node_num) {}
        void log_message(const std::string& module, const std::string& message);
        void log_error(const std::string& module, const std::string& error);

    private:
        std::string get_formatted_time();
        node node_num;
};

#endif