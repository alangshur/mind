#include <cassert>
#include "util/semaphore.hpp"

BaseSemaphore::BaseSemaphore(int count) : m_count(count) { 
    assert(count > -1); 
}

void BaseSemaphore::post() {
    std::unique_lock<std::mutex> lock(this->m_mutex);
    ++this->m_count;
    this->m_cv.notify_one();
}

void BaseSemaphore::wait() {
    std::unique_lock<std::mutex> lock(this->m_mutex);
    this->m_cv.wait(lock, [this]() { return this->m_count != 0; });
    --this->m_count;
}

EffSemaphore::EffSemaphore(int count) : m_count(count), 
    m_semaphore(0) {}

void EffSemaphore::post() {
    int count = m_count.fetch_add(1, std::memory_order_release);
    if (count < 0) m_semaphore.post();
}

void EffSemaphore::wait() {
    int count = m_count.fetch_sub(1, std::memory_order_acquire);
    if (count < 1) m_semaphore.wait();
}
