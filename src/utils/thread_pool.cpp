#include "../../include/utils/thread_pool.hpp"

thread_pool::thread_pool(std::size_t worker_count) {
    for (size_t i = 0; i < worker_count; i++)
    {
        workers.emplace_back([&] (const std::stop_token &stop_tok) {
            while (!stop_tok.stop_requested())
            {
                std::unique_lock lock(mutex);                    
                if (queue.empty())
                {
                    if (stop)
                    {
                        return;
                    }
                    continue;
                }
                auto task = std::move(queue.front());
                queue.pop();
                lock.unlock();
                
                std::invoke(task);
                unfinished--;
                
                if (unfinished.load() == 0 && stop)
                {
                    work_done.release();
                    return;
                }
            }
        });
    }
}

thread_pool::~thread_pool() {
    this->join();

     for (auto &&worker : workers)
    {
        worker.request_stop();
        worker.join();
    }
    
}

void thread_pool::join() {
    stop = true;
    // impl have this thread also work maybe? so not to spin
    while (unfinished.load() > 0)
    {
        work_done.acquire();
    }   
}

