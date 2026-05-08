#include "BitsetD.hpp"
#include <stdexcept>
#include <cassert>
#include <algorithm>

BitsetD::BitsetD(const BitsetD& src) {
    size_ = src.size();
    bits_ = src.bits_;
   
}


BitsetD::BitsetD(BitsetD&& src) noexcept
{
    size_ = src.size_;
    bits_ = std::move(src.bits_);
    src.size_ = 0;
}

BitsetD& BitsetD::operator=(BitsetD&& rhs) noexcept
{
    if (this != &rhs) {
        size_ = rhs.size_;
        bits_ = std::move(rhs.bits_);
        rhs.size_ = 0;
    }
    return *this;
}

BitsetD::BitsetD(const std::int32_t size, const bool val)
{
    if (size <= 0) {
        throw std::invalid_argument("Size <= 0");
    }
    size_ = size;
    std::int32_t word_count = (size + 31) / 32;
    bits_.resize(word_count, val ? 0xFFFFFFFFu : 0u);

    if (val && (size_ % 32 != 0)) {
        std::uint32_t tail_mask = (1U << (size_ % 32)) - 1;
        bits_.back() &= tail_mask;
    }
}

BitsetD::BitsetD(const std::uint64_t mask, const int32_t size)
{
    // TODOOOOO
    if (size <= 0){
        throw std::invalid_argument("BitsetD constructor: size cannot be negative");
    }
    size_ = size;
    int32_t word_count = (size + 31)/32;
    bits_.resize(word_count, 0u);

    if (word_count > 0) {
        bits_[0] = static_cast<std::uint32_t>(mask & 0xFFFFFFFF);
    }
    if (word_count > 1) {
        bits_[1] = static_cast<std::uint32_t>((mask >> 32) & 0xFFFFFFFF);
    }

    if (size_ % 32 != 0) {
        std::uint32_t tail_mask = (1U << (size_ % 32)) - 1;
        bits_.back() &= tail_mask;
    }
}


void BitsetD::set(const int32_t index, bool val)
{
    if (index < 0 || index >= size_) {
        throw std::out_of_range("BitsetD::set: index out of range");
    }

    int32_t word = index / 32;
    int32_t bit = index % 32;
    
    if (val){
        bits_[word] |= (1u << bit);
    } else {
        bits_[word] &= ~(1u << bit);
    }
}

BitsetD& BitsetD::invert() noexcept
{   
    if (size_ == 0) return *this;
    for (size_t  i = 0; i < bits_.size(); ++i) {
        bits_[i] = ~bits_[i];
    }
    if (size_ % 32 != 0) {
        bits_.back() &= (1u << (size_ % 32)) - 1;
    }
    return *this;
}

bool BitsetD::get(int32_t index) const
{
    if (index < 0 || index >= size_) {
        throw std::out_of_range("BitsetD::get: index out of range");
    }
    int32_t wordindex = index / 32;
    int32_t indexposition = index % 32;
    return (bits_[wordindex] & (1U << indexposition)) != 0;
}

void BitsetD::fill(const bool val) noexcept 
{
    assert(size_ >= 0 && "BitsetD::fill: size_ cannot be negative");

    if (size_ == 0) return;

    // int32_t word = size_ / 32 + bool(size_ % 32);
    // databasa.resize(word);
    if (val){
        for (uint64_t i = 0; i < bits_.size(); i++){
            // databasa[i] = (1ULL<<32) - 1;
            bits_[i] = 0xFFFFFFFFu;
        }
        if (size_ % 32 != 0){
            bits_.back() &= (1u <<(size_%32)) - 1;
        }
    }else{
        for (uint64_t i = 0; i < bits_.size(); i++){
            bits_[i] = 0;
        }
    }
}

void BitsetD::resize(const std::int32_t new_size, bool val)
{
    if (new_size <= 0) {
        throw std::invalid_argument("resize: new_size <= 0");
    }

    // assert(new_size >= 0 && "BitsetD::resize: size cannot be negative");
    // if (new_size == size()) return;

    std::int32_t old_size = size_;
    std::int32_t word_count = (new_size + 31) / 32;
    bits_.resize(word_count, 0u);
    size_ = new_size;

    if (new_size > old_size && val) {
        for (std::int32_t i = old_size; i < new_size; ++i) {
            this -> set(i, val);
        }
    }
    if (size_ % 32 != 0) {
        bits_.back() &= (1U << (size_ % 32)) - 1;
    }
}

bool BitsetD::operator==(const BitsetD& rhs) const noexcept
{
    if (size() != rhs.size_) {
        return false;
    } else {
        for (int32_t i = 0; i < bits_.size(); i++) {
            if (bits_[i] != rhs.bits_[i]) {
                return false;
            }
        }
    }
    return true;
}

std::ostream & BitsetD::write_to(std::ostream & strm) const
{
    strm.put('b');
    strm.put(static_cast<char>(size_));
    int total_bytes = (size_ + 7) / 8;
    char hash = 0;

    for (int i = 0; i < total_bytes; ++i) {
        char byte = 0;
        for (int b = 0; b < 8; ++b) {
            int bit_idx = i * 8 + b;
            if (bit_idx < size_ && get(bit_idx)) {
                byte |= (1 << b);
            }
        }
        strm.put(byte);
        hash ^= byte;
    }

    strm.put(hash);
    return strm;
}

std::istream & BitsetD::read_from(std::istream & strm)
{
    char marker;
    strm.get(marker);
    if (marker != 'b') {
        strm.setstate(std::ios::failbit);
        return strm;
    }

    char size_char;
    strm.get(size_char);
    int32_t new_size = static_cast<uint8_t>(size_char);

    int total_bytes = (new_size + 7) / 8;
    std::vector<uint32_t> new_bits((new_size + 31) / 32, 0u);
    char calculated_hash = 0;

    for (int i = 0; i < total_bytes; ++i) {
        char byte;
        strm.get(byte);
        calculated_hash ^= byte;

        int word_idx = (i * 8) / 32;
        int shift = (i * 8) % 32;
        new_bits[word_idx] |= (static_cast<uint32_t>(static_cast<uint8_t>(byte)) << shift);
    }

    char stream_hash;
    strm.get(stream_hash);

    if (calculated_hash != stream_hash) {
        strm.setstate(std::ios::failbit); 
        return strm;
    }

    size_ = new_size;
    bits_ = std::move(new_bits);
 
    if (size_ % 32 != 0) {
        bits_.back() &= (1U << (size_ % 32)) - 1;
    }
    
    return strm;
}

std::string BitsetD::to_string(const StrFormat fmt, const int32_t len) const
{
    if (len < 0) {
        throw std::invalid_argument("to_string: len < 0");
    }
    if (size_ == 0) {
        return "empty";
    }

    std::string result = "";

    if (fmt == StrFormat::Bin || fmt == StrFormat::BinNoPreSep) {
        if (fmt == StrFormat::Bin) result = "0b";

        for (int32_t i = size_ - 1; i >= 0; --i) {
            result += get(i) ? '1' : '0';

            if (fmt == StrFormat::Bin && i > 0 && i % 4 == 0) {
                result += '\'';
            }
        }
    } 
    //TODO:  Oct по аналогии с Hex

    if (len > 0 && (int32_t)result.length() < len) {
        result.insert(fmt == StrFormat::Bin ? 2 : 0, len - result.length(), '0');
    }

    return result;
}

std::int32_t BitsetD::size() const noexcept
{
    return size_;
}

BitsetD& BitsetD::shift(const std::int32_t idx) noexcept
{
    if (size_ == 0) return *this;

    int32_t k = idx;
    k = k % size();
    if (k == 0){
        return *this;
    }
    BitsetD bitset(size(), false);
    for (int32_t i = 0; i < size_; ++i) {
        int32_t old_p = i;
        int32_t new_p = (i+k)%size();
        if (new_p < 0) {
            new_p += size();
        }
        if (get(old_p)) {
            bitset.set(new_p, true);
        }
    }
    *this = bitset;
    return *this;
}


BitsetD& BitsetD::operator<<=(const std::int32_t shift) {
    if (shift < 0) {
        throw std::invalid_argument("negative shift");
    }
    if (shift == 0 || size_ == 0) return *this;
    if (shift >= size_) {
        fill(false);
        return *this;
    }

    for (int32_t i = size_ - 1; i >= shift; --i) {
        set(i, get(i - shift));
    }

    for (int32_t i = 0; i < shift; ++i) {
        set(i, false);
    }
    return *this;
}


BitsetD& BitsetD::operator>>=(const std::int32_t shift) {
    if (shift < 0) {
        throw std::invalid_argument("negative shift");
    }
    if (shift == 0 || size_ == 0) return *this;
    
    if (shift >= size_) {
        fill(false);
        return *this;
    }

    for (int32_t i = 0; i < size_ - shift; ++i) {
        set(i, get(i + shift));
    }
    for (int32_t i = size_ - shift; i < size_; ++i) {
        set(i, false);
    }
    return *this;
}
BitsetD& BitsetD::operator&=(const BitsetD& rhs)
{
    if (size_ != rhs.size_) {
        throw std::invalid_argument("operator&=: sizes must be equal");
    }

    for (int32_t i = 0; i < bits_.size(); ++i) {
        bits_[i] &= rhs.bits_[i];
    }
    return *this;
}


BitsetD& BitsetD::operator|=(const BitsetD& rhs) {
    if (size_ != rhs.size_) {
        throw std::invalid_argument("operator|=: sizes must be equal");
    }
    for (size_t i = 0; i < bits_.size(); ++i) {
        bits_[i] |= rhs.bits_[i];
    }
    return *this;
}


BitsetD& BitsetD::operator^=(const BitsetD& rhs) {
    if (size_ != rhs.size_) {
        throw std::invalid_argument("operator^=: sizes must be equal");
    }
    for (size_t i = 0; i < bits_.size(); ++i) {
        bits_[i] ^= rhs.bits_[i];
    }
    return *this;
}

BitsetD operator<<(const BitsetD& lhs, const std::int32_t shift)
{
    if (shift < 0) {
        throw std::invalid_argument("BitsetD::operator<<: shift cannot be negative");
    }
    if (shift == 0 || lhs.size() == 0) {
        return BitsetD(lhs);
    }
    if (shift >= lhs.size()) {
        return BitsetD(lhs.size(), false);
    }
    
    BitsetD result(lhs.size(), false);
    for (int32_t i = 0; i < lhs.size() - shift; ++i) {
        if (lhs.get(i)) {
            result.set(i + shift, true);
        }
    }
    return result;
}

BitsetD operator>>(const BitsetD &lhs, const std::int32_t shift)
{
    if (shift < 0) {
        throw std::invalid_argument("BitsetD::operator>>: shift cannot be negative");
    }
    if (shift == 0 || lhs.size() == 0) {
        return BitsetD(lhs);
    }
    if (shift >= lhs.size()) {
        return BitsetD(lhs.size(), false);
    }
    
    BitsetD result(lhs.size(), false);
    for (int32_t i = shift; i < lhs.size(); ++i) {
        if (lhs.get(i)) {
            result.set(i - shift, true);
        }
    }
    return result;
}


BitsetD operator&(const BitsetD &lhs, const BitsetD &rhs)
{
    BitsetD result(lhs);
    result &= rhs;
    return result;
}


BitsetD operator|(const BitsetD &lhs, const BitsetD &rhs)
{
    BitsetD result(lhs);
    result |= rhs;
    return result;
}


BitsetD operator^(const BitsetD &lhs, const BitsetD &rhs)
{
    BitsetD result(lhs);
    result ^= rhs;
    return result;
}


