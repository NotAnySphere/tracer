#if !defined(ARENA)
#define ARENA

#include <vector>
#include <cstring>
#include <cassert>
#include <stdlib.h>
#include <memory>

#include <iostream>

typedef char arena_item;

struct arena_page {
    arena_page* next_page;
    arena_item* buf;
};

class arena
{
private:
    arena_page* first;
    arena_page* page;
    size_t page_size;
    arena_item* index;

    size_t capacity;

    // std::vector<arena_item_record> only seems necessary for iterators?

    void alloc_new_page();

    void next_page();

public:
    arena(size_t page_size);
    ~arena();

    arena_item* alloc_item(size_t item_size);

    template<typename T, typename... Args>
    T* emplace_item(Args&&... args);

    void clean_arena();
};

arena::arena(size_t size)
{
    first = (arena_page*) malloc(sizeof(arena_page));
    page = first;
    page->buf = (arena_item*) malloc(size);
    page->next_page = nullptr;
    page_size = size;
    index = page->buf;
    capacity = size;
    // std::cout << "arena initialized\n" << std::endl;
}

arena::~arena()
{
    arena_page* head = first;
    arena_page* next = nullptr;
    do {
        free(head->buf);
        next = head->next_page;
        free(head);
        head = next;
    } while (next != nullptr);
}

void arena::alloc_new_page()
{
    auto next_page = page->next_page;
    while (next_page != nullptr)
    {
        next_page = next_page->next_page;
    }
    // std::cout << "got last" << std::endl;
    
    arena_item* new_buf = (arena_item*) malloc(page_size);
    arena_page* new_page = (arena_page*) malloc(sizeof(arena_page));
    new_page->next_page = nullptr;
    new_page->buf = new_buf;
    
    next_page = new_page;

    page->next_page = new_page;
    
    page = new_page;
    capacity = page_size;
    index = new_buf;
    // std::cout << "new page alloced.." << std::endl;

}

void arena::next_page()
{
    if (page->next_page == nullptr)
    {
        // std::cout << "allocing new page.." << std::endl;
        arena::alloc_new_page();
    }
    else
    {
        // std::cout << "found empty page.." << std::endl;
        page = page->next_page;
        index = page->buf;
        capacity = page_size;
    }
}

arena_item* arena::alloc_item(size_t item_size)
{
    assert(item_size <= page_size);
 
    // std::cout << "arena allo_item: " << item_size << " with capacity: " << capacity << std::endl;

    if (item_size > capacity)
    {
        arena::next_page();
    }
    
    arena_item* item_index = index;

    // #pragma warning
    index += item_size;
    capacity -= item_size;

    return item_index;
}

template<typename T, typename... Args>
T* arena::emplace_item(Args&&... args)
{
    auto mem = (T*) arena::alloc_item(sizeof(T));
    return new (mem) T(std::forward<Args>(args)...);
}

void arena::clean_arena()
{
    page = first;
    index = first->buf;
    capacity = page_size;
}

#endif // ARENA
