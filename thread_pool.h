#if !defined(THREAD_POOL)
#define THREAD_POOL

#include <mutex>
#include <condition_variable>
#include <queue>
#include <thread>
#include <vector>
#include <functional>

class thread_pool {
    std::queue<std::function<void()>> queue;
    std::vector<std::thread> workers;
    std::mutex mutex;
    std::condition_variable cv;

    thread_pool(std::size_t worker_count) {
        for (size_t i = 0; i < worker_count; i++)
        {
            workers.emplace_back([&](void){
        
                while (condition)
                {
                    std::scoped_lock lock(mutex);
                    auto task = std::move(queue.front());
                    queue.pop();
                    std::invoke(task);
                }
                    
                
            });
        }
    };

    ~thread_pool() {}

    template <typename F, typename... Args>
    void enqueue(F&& f, Args&&... args) {
        std::scoped_lock lock(mutex);
        queue.push([func = std::forward<F>(f), ... largs = std::forward<Args>(args)] () {
            std::invoke(func, largs...);
        });
        cv.notify_one();
    }

    // void join() {}

};


#endif // THREAD_POOL
