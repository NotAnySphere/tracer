#if !defined(THREAD_POOL)
#define THREAD_POOL

#include <mutex>
#include <semaphore>
#include <atomic>
#include <condition_variable>
#include <queue>
#include <thread>
#include <vector>
#include <functional>

class thread_pool {
    public:
    std::queue<std::function<void()>> queue {};
    std::vector<std::jthread> workers {};
    std::mutex mutex;
    std::binary_semaphore work_done { 0 };
    std::atomic_int32_t unfinished { 0 };
    bool stop = false;

    thread_pool(std::size_t worker_count) {
        for (size_t i = 0; i < worker_count; i++)
        {
            workers.emplace_back([&] (const std::stop_token &stop_tok) {
                while (!stop_tok.stop_requested())
                {
                    std::unique_lock lock(mutex);                    
                    if (queue.size() == 0 && stop)
                    {
                        return;
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
    };

    ~thread_pool() {
        this->join();

        for (auto &&worker : workers)
        {
            worker.request_stop();
            worker.join();
        }
        
    }

    template <typename F, typename... Args>
    void enqueue(F&& f, Args&&... args) {
        std::scoped_lock lock(mutex);
        queue.emplace([func = std::forward<F>(f), ... largs = std::forward<Args>(args)] () {
            std::invoke(func, largs...);
        });
        unfinished++;
        stop = false;
    }

    void join() {
        stop = true;
        // impl have this thread also work maybe? so not to spin
        while (unfinished.load() > 0)
        {
            work_done.acquire();
        }   
    }

};


#endif // THREAD_POOL
