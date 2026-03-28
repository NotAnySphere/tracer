#if !defined(ARENA)
#define ARENA

#include <cassert>
#include <cstring>
#include <memory>
#include <stdlib.h>
#include <vector>

#include <iostream>

#include "../utils/logger_alloc.hpp"

const size_t calculate_padding(size_t base, size_t alignment) {
    size_t factor = (base / alignment) + 1;
    size_t aligned_address = factor * alignment;
    size_t needed_padding = aligned_address - base;
    return needed_padding;
}

typedef char arena_item;

struct arena_page {
    arena_page *next_page;
    arena_item *buf;
};

class arena {
  private:
    arena_page *first;
    arena_page *page;
    size_t page_size;
    arena_item *index;

    size_t capacity;

    // std::vector<arena_item_record> only seems necessary for iterators?

    void alloc_new_page();

    void next_page();

  public:
    arena(size_t page_size);
    ~arena();

    arena_item *alloc_item(size_t item_size, size_t alignment);

    template <typename T, typename... Args> T *emplace_item(Args &&...args);

    void clean_arena();
};

arena::arena(size_t size) {
    first = (arena_page *)malloc(sizeof(arena_page));
    page = first;
    page->buf = (arena_item *)malloc(size);
    page->next_page = nullptr;
    page_size = size;
    index = page->buf;
    capacity = size;
    // std::cout << "arena initialized\n" << std::endl;
}

arena::~arena() {
    std::cout << "deconstructing arena...\n";
    arena_page *head = first;
    arena_page *next = nullptr;
    do {
        free(head->buf);
        next = head->next_page;
        free(head);
        head = next;
    } while (next != nullptr);
}

void arena::alloc_new_page() {
    auto next_page = page->next_page;
    while (next_page != nullptr) {
        next_page = next_page->next_page;
    }
    // std::cout << "got last" << std::endl;

    arena_item *new_buf = (arena_item *)malloc(page_size);
    arena_page *new_page = (arena_page *)malloc(sizeof(arena_page));
    new_page->next_page = nullptr;
    new_page->buf = new_buf;

    next_page = new_page;

    page->next_page = new_page;

    page = new_page;
    capacity = page_size;
    index = new_buf;
    // std::cout << "new page alloced.." << std::endl;
}

void arena::next_page() {
    if (page->next_page == nullptr) {
        // std::cout << "allocing new page.." << std::endl;
        arena::alloc_new_page();
    } else {
        // std::cout << "found empty page.." << std::endl;
        page = page->next_page;
        index = page->buf;
        capacity = page_size;
    }
}

arena_item *arena::alloc_item(size_t item_size, size_t alignment) {
    // TODO: currently also adding padding if already perfectly aligned
    size_t needed_padding = calculate_padding((size_t)index, alignment);

    auto total_item_size = needed_padding + item_size;

    assert(total_item_size <= page_size);

    if (total_item_size > capacity) {
        arena::next_page();
        needed_padding = calculate_padding((size_t)index, alignment);
    }

    arena_item *item_index = index + needed_padding;

    // #pragma warning
    index += total_item_size;
    capacity -= total_item_size;

    //std::cout << "arena allo_item: " << std::dec << item_size << " with capacity: " << capacity << " with padding: " << needed_padding << std::hex << " at address: 0x" << (size_t) item_index << " with padded_address: " << (size_t) index << std::dec << "\n";
    return item_index;
}

template <typename T, typename... Args> T *arena::emplace_item(Args &&...args) {
    auto mem = (T *)arena::alloc_item(sizeof(T), alignof(T));
    // auto mem = (T*) arena::alloc_item(sizeof(T), alignof(T));

    return new (mem) T(std::forward<Args>(args)...);
}

void arena::clean_arena() {
    page = first;
    index = first->buf;
    capacity = page_size;
}

inline std::shared_ptr<arena> get_default_arena() {
    thread_local std::shared_ptr<arena> alloc = std::make_shared<arena>(4096);
    //std::cout << "default arena taken\n";
    return alloc;
}

template <typename T> class ArenaAllocator {
  public:
    using value_type = T;

    std::shared_ptr<arena> pool;

    ArenaAllocator(std::shared_ptr<arena> a) noexcept : pool(a) {}
    
    ArenaAllocator() noexcept : pool(get_default_arena()) {}
    
    ArenaAllocator(const size_t size) {
        pool = std::move(std::make_shared<arena>(size));
    }

    template <typename U>
    ArenaAllocator(const ArenaAllocator<U> &other) noexcept
        : pool(other.pool) {}

    T *allocate(std::size_t n) {
        return static_cast<T *>(
            (void *)pool->alloc_item(n * sizeof(T), alignof(T)));
    }

    void deallocate(T *, std::size_t) noexcept {}

    void clear() noexcept {
        pool->clean_arena();
    }

    bool operator==(const ArenaAllocator &other) const noexcept {
        return pool == other.pool;
    }

    bool operator!=(const ArenaAllocator &other) const noexcept {
        return !(*this == other);
    }
};

#endif // ARENA
