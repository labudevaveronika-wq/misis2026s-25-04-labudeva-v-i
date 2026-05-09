#ifndef STACKA_LIBRARY_H
#define STACKA_LIBRARY_H
#include <iostream>
#include <cstdint>

class stacka {
public:
    stacka() = default;

    ~stacka();

    stacka(const stacka &);

    stacka &operator=(const stacka &);

    bool is_empty() const noexcept;

    void pop() noexcept;

    void push(const int64_t val);

void resize(const size_t new_size);

    int64_t& top();
    int64_t top() const;

    void clear() noexcept;

private:
    size_t size=0;
    size_t capacity = 0;
    int64_t* data = nullptr;

};

#endif