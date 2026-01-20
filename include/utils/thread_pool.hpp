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

    thread_pool(std::size_t worker_count);

    ~thread_pool();

    template <typename F, typename... Args>
    void enqueue(F&& f, Args&&... args) {
        std::scoped_lock lock(mutex);
        queue.emplace([func = std::forward<F>(f), ... largs = std::forward<Args>(args)] () {
            std::invoke(func, largs...);
        });
        unfinished++;
        stop = false;
    }

    void join();
};


#endif // THREAD_POOL
