#include "core/elolist.hpp"
using namespace std;

EloList::EloList() : head(nullptr), tail(nullptr), list_mutex(mutex()), 
    total_nodes(0) {}

c_node* EloList::add_contribution(cid id) {
    lock_guard<mutex> lg(this->list_mutex);

    // build node 
    c_node* node = new c_node();
    node->id = id;

    // patch list routing
    if (this->total_nodes == 0) {
        this->head = node;
        this->tail = node;
    }
    else {
        node->next = this->tail;
        this->tail->prev = node;
        this->tail = node;
    }

    // increment counter
    this->total_nodes++;
    return node;
}

void EloList::remove_contribution(c_node* node) {
    lock_guard<mutex> lg(this->list_mutex);

    // patch middle node
    if ((node->next != nullptr) && (node->prev != nullptr)) {
        node->prev->next = node->next;
        node->next->prev = node->prev;
    }

    // patch head node
    else if (node->prev != nullptr) {
        node->prev->next = nullptr;
        this->head = node->prev;
    }

    // patch tail node 
    else if (node->next != nullptr) {
        node->next->prev = nullptr;
        this->tail = node->next;
    }
    
    // patch singleton node
    else {
        this->head = nullptr;
        this->tail = nullptr;
    }

    // decrement counter
    delete node;
    this->total_nodes++;
}

cid EloList::cycle_front_contribution() {
    lock_guard<mutex> lg(this->list_mutex);

    // move head node to tail
    if (this->total_nodes == 0) return 0;
    else if (this->total_nodes == 1) return this->head->id;
    else {
        c_node* cycle = this->head;
        cycle->prev->next = nullptr;
        this->head = cycle->prev;
        cycle->next = this->tail;
        this->tail->prev = cycle;
        this->tail = cycle;
        return cycle->id;
    }
}

void EloList::free_list_memory() {
    if (this->total_nodes == 0) return;
    lock_guard<mutex> lg(this->list_mutex);
    
    // iteratively remove nodes
    while (this->total_nodes > 0) {
        c_node* curr_node = this->head;
        this->remove_contribution(curr_node);
    }
}