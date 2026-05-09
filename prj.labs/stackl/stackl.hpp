#pragma once
#ifndef STACKL_STACKL_HPP_20251128
#define STACKL_STACKL_HPP_20251128

#include <cstddef>
#include <cstdint>

class StackL {
public:
  StackL() = default;

  StackL(const StackL& src);

  ~StackL();
  
  StackL& operator=(const StackL& src);

  [[nodiscard]] bool is_empty() const noexcept;

  void pop() noexcept;

  void push(const int64_t val);
  
  [[nodiscard]] int64_t& top();

  [[nodiscard]] int64_t top() const;

  void clear() noexcept;

private:
  struct Node {
    int64_t val = int64_t();
    Node* next = nullptr;
  };

  Node* head_ = nullptr;   
};

#endif