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
        BitsetD(BitsetD&& src) noexcept;


        BitsetD(const std::int32_t size, const bool val);
        BitsetD(const std::uint64_t mask, const int32_t size = 64);

        BitsetD& operator=(const BitsetD& rhs);
        BitsetD& operator=(BitsetD&& rhs) noexcept;

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

        explicit operator std::uint64_t() const;
        explicit operator std::uint32_t() const;

        enum class StrFormat : int32_t {
            Bin = 1,     
            BinNoPreSep,  
            Oct,
            Hex,
            Def = Bin,
            Default = Def
        };

        std::string to_string(const StrFormat fmt = StrFormat::Def, const int32_t len = 0) const;

        std::int32_t size() const noexcept 
        {
            return size_;
        }

        class BitR {
            friend class BitsetD;
        public:
                ~BitR() = default;
            operator bool() const noexcept { return val_; }
        private:
            BitR(const BitsetD& bs, const int32_t idx) : val_(bs.get(idx)) {}
            bool val_ = false;
            BitR() = delete;
            BitR(const BitR&) = delete;
            BitR& operator=(const BitR&) = delete;
        };
        class BitW {
            friend class BitsetD;
        public:
            ~BitW() = default;
            operator bool() const noexcept { return bs_.get(idx_); }
            void operator=(const bool val) noexcept { bs_.set(idx_, val); }
            void operator=(const BitW& rhs) noexcept { operator=(rhs.operator bool()); }
        private:
            BitW(BitsetD& bs, const int32_t idx) : bs_(bs), idx_(idx) {}
            BitsetD& bs_;
            const int32_t idx_;
            BitW() = delete;
            BitW(const BitW&) = delete;
        };
        BitW operator[](const std::int32_t idx) & { return BitW(*this, idx); }
        BitR operator[](const std::int32_t idx) const & { return BitR(*this, idx); }

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