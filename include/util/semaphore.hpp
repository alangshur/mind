#ifndef SEMAPHORE_H
#define SEMAPHORE_H

#include <mutex>
#include <atomic>
#include <condition_variable>

/*
    The BaseSemaphore class implements a naive version of 
    the semaphore concurrency construct.
*/
class BaseSemaphore {
    public:
        BaseSemaphore(int count);
        void post();
        void wait();

    private:
        int m_count;
        std::mutex m_mutex;
        std::condition_variable m_cv;
};

/*
    The EffSemaphore class implements a more advanced 
    and much faster version of the naive semaphore using
    concurrency directives from the new C++ thread package.
*/
class EffSemaphore {
    public:
        EffSemaphore();
        EffSemaphore(int count);
        void post();
        void wait();

    private:
        std::atomic<int> m_count;
        BaseSemaphore m_semaphore;
};

#endif
