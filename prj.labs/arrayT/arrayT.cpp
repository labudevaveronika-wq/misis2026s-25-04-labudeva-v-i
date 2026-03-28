#include "ArrayT.hpp"
#include <algorithm>
#include <string>

template<typename T, typename S>
ArrayT<T, S>::ArrayT(const ArrayT& src) 
    : capacity_(src.size_)
    , size_(capacity_)
    , data_(new T[size_]) {
    std::copy(data_, src.data_, size_ * sizeof(*data_));
}
  
template<typename T, typename S>
ArrayT<T, S>::ArrayT(const S size)
    : capacity_(size)
    , size_(size) { 
    if (size_ < 0) {
        throw std::invalid_argument("ArrayT::ArrayT - negative size");
    }
    data_ = (capacity_ > 0) ? new T[capacity_]() : nullptr;
}

template<typename T, typename S>
ArrayT<T, S>::~ArrayT() {
    delete[] data_;
}
  
template<typename T, typename S>
ArrayT<T, S>& ArrayT<T, S>::operator=(const ArrayT& rhs) {
    if (this != & rhs) {
        resize(rhs.size_);
        std::memcpy(data_, rhs.data_, size_ * sizeof(*data_));
    }
    return *this;
}
 
template<typename T, typename S>
void ArrayT<T, S>::resize(const S size) {
    if (size < 0) {
        throw std::invalid_argument("ArrayT::resize - non positive size");
    }
    if (capacity_ < size) {
        auto data = new T[size]{0.0};
        if (0 < size_) {
            std::memcpy(data, data_, size_ * sizeof(*data_));
        }
        std::swap(data_, data);
        delete[] data;
        capacity_ = size;
    } else {
        if (size_ < size) {
            std::memset(data_ + size_, 0, (size - size_) * sizeof(*data_));
        }
    }
    size_ = size;
}
  
template<typename T, typename S>
T& ArrayT<T, S>::operator[](const S idx) {
    if (idx < 0 || size_ <= idx) {
        throw std::invalid_argument("ArrayT::operator[] - invalid index");
    }
    return *(data_ + idx);
}

template<typename T, typename S>
T ArrayT<T, S>::operator[](const S idx) const {
    if (idx < 0 || size_ <= idx) {
        throw std::invalid_argument("ArrayT::operator[] - invalid index");
    }
    return data_[idx];
}

template<typename T, typename S>
void ArrayT<T, S>::insert(const S idx, const T val) {
    if (idx < 0 || size_ < idx) {
        throw std::invalid_argument("ArrayT::Insert - invalid index");
    }
    resize(size_ + 1);
    if (idx != size() - 1) {
        std::memmove(data_ + idx + 1, data_ + idx, (size_ - idx - 1) * sizeof(T));
    }
    data_[idx] = val;
}

template<typename T, typename S>
void ArrayT<T, S>::remove(const S idx) {
    if (idx < 0 || size_ <= idx) {
        throw std::invalid_argument("ArrayT::operator[] - invalid index");
    }
    if (idx != size_ - 1) {
        // удаляем НЕ в конце
        std::memmove(data_ + idx, data_ + idx + 1, (size_ - idx) * sizeof(T));
    }
    resize(size_ - 1);
}