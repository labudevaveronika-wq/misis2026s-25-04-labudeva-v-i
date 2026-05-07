#include "ArrayT.hpp"
#include <algorithm>
#include <string>


template<typename T, typename S>
ArrayT<T, S>::ArrayT()
    : capacity_(0), size_(0), data_(nullptr) {}

    
template<typename T, typename S>
ArrayT<T, S>::ArrayT(const ArrayT& src) 
    : capacity_(src.size_)
    , size_(capacity_)
    , data_(src.size_ > 0 ? new T[src.size_] : nullptr) {
    for (S i = 0; i < size_; ++i) {
        data_[i] = src.data_[i];
    }
}
  
template<typename T, typename S>
ArrayT<T, S>::ArrayT(const S size) {
    if (size < 0) throw std::invalid_argument("ArrayT: negative size");
    size_ = size;
    capacity_ = size;
    data_ = (size > 0) ? new T[size]() : nullptr;
}

template<typename T, typename S>
ArrayT<T, S>::~ArrayT() {
    delete[] data_;
}
  
template<typename T, typename S>
ArrayT<T, S>& ArrayT<T, S>::operator=(const ArrayT& rhs) {
    if (this != &rhs) {
        T* new_data = nullptr;
        if (rhs.size_ > 0) {
            new_data = new T[rhs.size_];
            for (S i = 0; i < rhs.size_; ++i) {
                new_data[i] = rhs.data_[i];
            }
        }
        delete[] data_;
        data_ = new_data;
        size_ = rhs.size_;
        capacity_ = rhs.size_;
    }
    return *this;
}
 
template<typename T, typename S>
void ArrayT<T, S>::resize(const S size) {
    if (size < 0) throw std::invalid_argument("ArrayT: negative size");
    
    if (size > capacity_) {
        S new_capacity = (capacity_ > 0) ? capacity_ * 2 : size;
        if (new_capacity < size) new_capacity = size;

        T* new_data = new T[new_capacity]();
        for (S i = 0; i < size_; ++i) {
            new_data[i] = std::move(data_[i]);
        }
        delete[] data_;
        data_ = new_data;
        capacity_ = new_capacity;
    }
    size_ = size;
}
  
template<typename T, typename S>
T& ArrayT<T, S>::operator[](const S idx) {
    if (idx < 0 || idx >= size_) {
        throw std::out_of_range("ArrayT: index out of range");
    }
    return data_[idx];
}

template<typename T, typename S>
const T& ArrayT<T, S>::operator[](const S idx) const {
    if (idx < 0 || idx >= size_) {
        throw std::out_of_range("ArrayT: index out of range");
    }
    return data_[idx];
}

template<typename T, typename S>
void ArrayT<T, S>::insert(const S index, const T val) {
    if (index < 0 || index > size_) throw std::out_of_range("ArrayT: invalid index");
    
    S old_size = size_;
    resize(size_ + 1);
    for (S i = old_size; i > index; --i) {
        data_[i] = std::move(data_[i - 1]);
    }
    data_[index] = val;
}

template<typename T, typename S>
void ArrayT<T, S>::remove(const S idx) {
    if (idx < 0 || idx >= size_) throw std::out_of_range("ArrayT: invalid index");
    
    for (S i = idx; i < size_ - 1; ++i) {
        data_[i] = std::move(data_[i + 1]);
    }
    size_--;
}