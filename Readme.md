# CPP_TOOLS
## LibLoader
A little plugin loader in c++ for dynamic library (cross-plateform Linux/Windows)

## ThreadPool
A c++11 thread pool pattern:
class ThreadPool:
- ThreadPool(size)
- AddTask(function / lambda, scopelock, args ...) -> std::future
 |-> function : function to compute by a thread.
 |-> scopelock : lock (or not) the function scope and variable used.
 |-> args ... : variatic parameter for the function.

Modified from this project: https://github.com/progschj/ThreadPool.git
