#if !defined(LOGGER_ALLOC)
#define LOGGER_ALLOC

#include <iostream>

template <typename T> class LoggingAllocator {
  public:
    using value_type = T;

    std::allocator<T> alloc;


    LoggingAllocator() {}

    T *allocate(std::size_t n) {
        std::cout << "<LoggingAllocator> Allocated " << n * sizeof(T) << " bytes of " << typeid(T).name() << "\n";        
        
        return static_cast<T *>(
            (void *)alloc.allocate(n));
    }

    void deallocate(T* p, std::size_t n) noexcept {
        std::cout << "<LoggingAllocator> Deallocated " << n * sizeof(T) << " bytes of " << typeid(T).name() << "\n";        

        alloc.deallocate(p,n);
    }

    bool operator==(const LoggingAllocator &other) const noexcept {
        return true;
    }

    bool operator!=(const LoggingAllocator &other) const noexcept {
        return !(*this == other);
    }
};

#endif // LOGGER_ALLOC
