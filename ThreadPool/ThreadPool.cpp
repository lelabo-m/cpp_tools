#include "ThreadPool.hpp"

ThreadPool::ThreadPool(size_t nb_thread) : _stop(false)
{
    auto generator = [this] { ThreadPool::MainLoop(this); };
    for(size_t i = 0; i < nb_thread; ++i)
        _workers.emplace_back(generator);
}

ThreadPool::~ThreadPool()
{
    {
        std::unique_lock<std::mutex> lock(_mutex);
        _stop = true;
    }
    _condvar.notify_all();
    for(auto& worker: _workers)
        worker.join();
}

void    ThreadPool::MainLoop(ThreadPool* threadpool)
{
    for(;;)
    {
        ThreadTask  task;
        bool        scopelock;
        {
            std::unique_lock<std::mutex> lock(threadpool->_mutex);
            threadpool->_condvar.wait(lock,
                [threadpool]{ return (threadpool->getStop() || !threadpool->isTaskEmpty()); });
            if(threadpool->getStop() && threadpool->isTaskEmpty())
                return;
            auto pack = std::move(threadpool->_tasks.front());
            scopelock = std::get<0>(pack);
            task = std::get<1>(pack);
            threadpool->_tasks.pop();
            if (scopelock)
                task();
        }
        if (!scopelock)
            task();
    }
}
