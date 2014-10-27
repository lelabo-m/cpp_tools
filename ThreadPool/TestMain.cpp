#include <iostream>
#include <vector>
#include <chrono>
#include "ThreadPool.hpp"

int main()
{

    ThreadPool pool(4);
    std::vector< std::future<int> > results;

    for(int i = 0; i < 8; ++i) {
        results.emplace_back(
            pool.AddTask([i] {
                std::cout << "hello " << i << std::endl;
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
                std::cout << "world " << i << std::endl;
                return i*i;
            }, false)
        );
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    std::cout << std::endl << "RESULT:" << std::endl;
    for(auto && result: results)
        std::cout << result.get() << ' ';
    std::cout << std::endl;

    return 0;
}