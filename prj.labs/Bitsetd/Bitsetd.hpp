#pragma once
#ifndef BITSETD_BITSETD_2026
#define BITSETD_BITSETD_2026

#include <cstddef>
#include <cstdint>
#include <vector>
#include <iostream>

class BitsetD {
    public:
        BitsetD() = default;
        ~BitsetD() = default;


        BitsetD(const BitsetD& src);
        BitsetD(const std::int32_t size, const bool val);
        BitsetD(const std::uint64_t mask, const int32_t size = 64);
        BitsetD& operator=(const BitsetD& rhs);

        void set(const int32_t index, const bool val);
        bool get(const int32_t index) const;

        void invert() noexcept;
        void fill(const bool val) noexcept;
        void resize(const std::int32_t new_size, bool val = false) noexcept;

        BitsetD& shift(const std::int32_t idx) noexcept;
        BitsetD& operator<<=(const std::int32_t shift);
        BitsetD& operator>>=(const std::int32_t shift);
        BitsetD& operator&=(const BitsetD& rhs);
        BitsetD& operator|=(const BitsetD& rhs);
        BitsetD& operator^=(const BitsetD& rhs);

        bool operator==(const BitsetD& rhs) const noexcept;

        std::int32_t size() const noexcept 
        {
            return size_;
        }



        class BitProx {
            public:
                BitProx() = delete;
                BitProx(const BitProx&) = delete;
                ~BitProx() = default;
                BitProx& operator=(const BitProx&) = delete;
                BitProx(BitsetD& bs, const int32_t idx) : bs_(bs), idx_(idx) {}
                operator bool() const { return bs_.get(idx_); }
                void operator=(const bool val) { bs_.set(idx_, val); }
            private:
                BitsetD& bs_;
                const int32_t idx_ = 0;
        };
        class BitProxC {
            public:
                BitProxC() = delete;
                BitProxC(const BitProxC&) = delete;
                ~BitProxC() = default;
                BitProxC& operator=(const BitProxC&) = delete;
                BitProxC(const BitsetD& bs, const int32_t idx) : val_(bs.get(idx)) {}
                operator bool() const { return val_; }
            private:
                bool val_ = false;

        // BitProx operator[](const std::int32_t idx) { return BitProx(*this, idx); }
        // BitProxC operator[](const std::int32_t idx) const { return BitProxC(*this, idx); }
  };


    private:

        std::int32_t size_ = 0;
        std::vector<std::uint32_t> databasa;
};


 
inline BitsetD operator~(const BitsetD& rhs) noexcept  { auto A = rhs; A.invert(); return A; };

BitsetD operator<<(const BitsetD& lhs, const std::int32_t shift);
  
BitsetD operator>>(const BitsetD& lhs, const std::int32_t shift);

BitsetD operator&(const BitsetD& lhs, const BitsetD& rhs);

BitsetD operator|(const BitsetD& lhs, const BitsetD& rhs);

BitsetD operator^(const BitsetD& lhs, const BitsetD& rhs);

#endif