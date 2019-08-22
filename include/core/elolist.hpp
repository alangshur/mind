#ifndef ELOLIST_H
#define ELOLIST_H

#include <mutex>
#include "orchestrator.hpp"

typedef struct c_node {
    cid id;
    struct c_node* next;
    struct c_node* prev;
} c_node;

/*
    The ELO list class is a manually-implemented doubly
    linked list used to efficiently order and rank
    contributions within individual ELO buckets.
*/
class EloList {
    public:
        EloList();
        c_node* add_contribution(cid id);
        void remove_contribution(c_node* node);
        cid cycle_front_contribution();
        void free_list_memory();
    
    private:
        c_node* head;
        c_node* tail;
        std::mutex list_mutex;
        uint32_t total_nodes;
};

#endif