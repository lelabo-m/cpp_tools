#ifndef THREADPOOL_H_
#define THREADPOOL_H_

#include <vector>
#include <queue>
#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <functional>

typedef std::vector<std::thread>                    ThreadVec;
typedef std::function<void ()>                      ThreadTask;
typedef std::queue< std::tuple<bool, ThreadTask> >  TaskQueue;

class ThreadPool
{
public:
    ThreadPool(size_t);
    ~ThreadPool();
    static void MainLoop(ThreadPool *);

    template<class F, class... Args>
    auto AddTask(F&& f, bool scopelock, Args&&... args) -> std::future<typename std::result_of<F(Args...)>::type>
    {
        using return_type = typename std::result_of<F(Args...)>::type; // Define Type
        auto func = std::bind(std::forward<F>(f), std::forward<Args>(args)...); // Wrap the function and the args
        auto task = std::make_shared< std::packaged_task<return_type()> >(func); // package it and turn into a smart ptr
        std::future<return_type> res = task->get_future();
        {
            std::unique_lock<std::mutex> lock(_mutex);
            if(!_stop)
                _tasks.emplace(std::make_tuple(scopelock, [task](){ (*task)(); }));
        }
        _condvar.notify_one();
        return res;
    }
    bool    getStop() { return _stop; };
    bool    isTaskEmpty() { return _tasks.empty(); };
private:
    ThreadVec               _workers;
    TaskQueue               _tasks;
    std::mutex              _mutex;
    std::condition_variable _condvar;
    bool                    _stop;
};

#endif /* !THREADPOOL_H_ */