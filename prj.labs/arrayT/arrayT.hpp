// 2026 by Polevoi Dmitry under Unlicense

#pragma once
#ifndef ARRAYT_ARRAYT_HPP_20260319
#define ARRAYT_ARRAYT_HPP_20260319

#include <cstddef>
#include <cstring>
#include <stdexcept>
#include <utility>

template<
  typename T,                 ///< value type
  typename S = std::ptrdiff_t ///< size type
>
class ArrayT {
    public:
        ArrayT() = default;
        ArrayT(const ArrayT&);
        //! \param size - начальный размер, 0 < size
        explicit ArrayT(const S size);
        ~ArrayT();
  
        ArrayT& operator=(const ArrayT& rhs);

        [[nodiscard]] S size() const noexcept { return size_; }
  
        //! \param size - новый размер, 0 <= size
        void resize(const S size);
  
        //! \param idx - индекс  элемента, 0 <= idx < Size()
        [[nodiscard]] T& operator[](const S idx);
        [[nodiscard]] const T& operator[](const S idx) const;

        //! \param idx - индекс вставляемого элемента, 0 <= idx <= size 
        void insert(const S idx, const T val);

        //! \param idx - индекс удаляемого элемента, 0 <= idx < size 
        void remove(const S idx);

    private:
        S capacity_ = 0;  //!< размер буффера
        S size_ = 0;      //!< число элементов в массиве
        T* data_ = nullptr;             //!< буффер
};

#endif // !ARRAYD_ARRAYD_HPP_20260319