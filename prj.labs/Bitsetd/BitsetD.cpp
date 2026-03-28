#include "BitsetD.hpp"
#include <stdexcept>
#include <cassert>

BitsetD::BitsetD(const BitsetD& src) {
    size_ = src.size_;
    databasa = src.databasa;
   
}

BitsetD::BitsetD(const std::int32_t size, const bool val)
{
    if (size <= 0) {
        throw std::invalid_argument("Size <= 0");
    }
    size_ = size;
    int32_t word_count = (size + 31) / 32;
    databasa.resize(word_count);
    fill(val);
}

BitsetD &BitsetD::operator=(const BitsetD& rhs)
{
    if (this != &rhs){
        size_ = rhs.size_;
        databasa = rhs.databasa;
    }
    return *this;
}

BitsetD::BitsetD(const std::uint64_t mask, const int32_t size)
{
    // TODOOOOO
    if (size <= 0){
        throw std::invalid_argument("BitsetD constructor: size cannot be negative");
    }
    int32_t word = size / 32 + bool(size % 32);
    size_ = size;
    databasa.resize(word);
    if (size < 64){
        if (size <= 32){
            databasa[0] = mask & ((1ULL<<size) - 1);
        }else{
            databasa[0] = mask & ((1ULL<<32) - 1);
            databasa[1] = mask & (((1ULL<<(size + 32)) - 1) - ((1ULL<<32) - 1));
        }
    }else{
        databasa[0] = mask & ((1ULL<<32) - 1);
        databasa[1] = mask & (((0xFFFFFFFFFFFFFFFF) - 1) - ((1ULL<<32) - 1));
        for (uint64_t i = 2; i < word; i++){
            databasa[i] = 0;
        }
    }
}



void BitsetD::set(const int32_t index, bool val)
{
    if (index < 0 || index >= size_) {
        throw std::invalid_argument("BitsetD::set: index out of range");
    }

    assert(index >= 0 && index < size_ && "BitsetD::set: index out of range");

    int32_t word = index / 32;
    int32_t bit = index % 32;
    
    if (val){
        databasa[word] |= (1u << bit);
    } else {
        databasa[word] &= ~(1u << bit);
    }
}


void BitsetD::invert() noexcept
{
    if (size_ == 0) return;
    for (uint32_t& word : databasa) {
        word = ~word;
    }
    if (size_ % 32 != 0) {
        databasa.back() &= (1u << (size_ % 32)) - 1;
    }
}

bool BitsetD::get(int32_t index) const
{
    if (index < 0 || index >= size_) {
        throw std::invalid_argument("BitsetD::get: index out of range");
    }
    int32_t wordindex = index / 32;
    int32_t indexposition = index % 32;
    uint32_t mask = 1u << indexposition;
    return (databasa[wordindex] & mask) != 0;
}

void BitsetD::fill(const bool val) noexcept 
{
    assert(size_ >= 0 && "BitsetD::fill: size_ cannot be negative");

    if (size_ == 0) return;

    // int32_t word = size_ / 32 + bool(size_ % 32);
    // databasa.resize(word);
    if (val){
        for (uint64_t i = 0; i < databasa.size(); i++){
            // databasa[i] = (1ULL<<32) - 1;
            databasa[i] = 0xFFFFFFFFu;
        }
        if (size_ % 32 != 0){
            databasa.back() &= (1u <<(size_%32)) - 1;
        }
    }else{
        for (uint64_t i = 0; i < databasa.size(); i++){
            databasa[i] = 0;
        }
    }
}

void BitsetD::resize(const std::int32_t new_size, bool val) noexcept
{

    assert(new_size >= 0 && "BitsetD::resize: size cannot be negative");
    if (new_size == size()) return;

    if (size() > 0 && size_ % 32 != 0 ) {
        databasa.back() &= (1u << (size_ % 32)) - 1;
    }

    int32_t word = new_size / 32 + bool(new_size % 32);
    int32_t old_size = size();
    databasa.resize(word, 0u);
    size_ = new_size;

    if (new_size > old_size) {
        for (int32_t i = old_size; i < new_size; i++) {
            this -> set(i, val);
        }
    }
    if (size_ % 32 != 0) {
        databasa.back() &= (1u << (size_ % 32)) - 1;
    }
}

bool BitsetD::operator==(const BitsetD& rhs) const noexcept
{
    assert(databasa.size() == rhs.databasa.size() && 
           "BitsetD::operator==: vector sizes mismatch");

    if (size() != rhs.size_) {
        return false;
    } else {
        for (int32_t i = 0; i < databasa.size(); i++) {
            if (databasa[i] != rhs.databasa[i]) {
                return false;
            }
        }
    }
    return true;
}

std::string BitsetD::to_string(const StrFormat fmt, const int32_t len) const
{
    if (size_ == 0) {
        return "empty";
    }

    std::string result = "";

    if (fmt == StrFormat::Bin || fmt == StrFormat::BinNoPreSep) {
        if (fmt == StrFormat::Bin) result = "b0";

        for (int32_t i = size_ - 1; i >= 0; --i) {
            if (get(i)) {
                result += '1';
            } else {
                result += '0';
            }

            bool is_bin_format = (fmt == StrFormat::Bin);
            bool is_every_fourth = (i % 4 == 0);
            bool is_not_last = (i > 0);
            
            if (is_bin_format && is_not_last && is_every_fourth) {
                result += '\'';
            }
        }
    } 
    //TODO:  Oct по аналогии с Hex

    if (len > 0 && (int32_t)result.length() < len) {
        result.insert(0, len - result.length(), '0');
    }

    return result;
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
    if (shift < 0) throw std::invalid_argument("negative shift");
    if (shift == 0) return *this;
    if (shift >= size_) { fill(false); return *this; }

    BitsetD result(size_, false);
    for (int32_t i = 0; i < size_ - shift; ++i) {
        if (get(i)) result.set(i + shift, true); // ВЛЕВО = УВЕЛИЧИТЬ индекс
    }
    *this = result;
    return *this;
}


BitsetD& BitsetD::operator>>=(const std::int32_t shift) {
    if (shift < 0) throw std::invalid_argument("negative shift");
    if (shift == 0) return *this;
    if (shift >= size_) { fill(false); return *this; }

    BitsetD result(size_, false);
    for (int32_t i = shift; i < size_; ++i) {
        if (get(i)) result.set(i - shift, true); // ВПРАВО = УМЕНЬШИТЬ индекс
    }
    *this = result;
    return *this;
}
BitsetD& BitsetD::operator&=(const BitsetD& rhs)
{
    if (size_ == 0) return *this;

    int32_t max_size = std::max(size_, rhs.size_);
    if (max_size > size_) {
        resize(max_size, false);
    }

    for (int32_t i = 0; i < databasa.size(); ++i) {
        if (i < rhs.databasa.size()) {
            databasa[i] &= rhs.databasa[i];
        } else {
            databasa[i] = 0;  // явно обнуляем
        }
    }
    return *this;
}


BitsetD& BitsetD::operator|=(const BitsetD& rhs) {
    if (rhs.size_ > size_) resize(rhs.size_, false);
    for (int32_t i = 0; i < rhs.size_; ++i) {
        if (rhs.get(i)) set(i, true);
    }
    return *this;
}


BitsetD& BitsetD::operator^=(const BitsetD& rhs) {
    if (rhs.size_ > size_) resize(rhs.size_, false);
    for (int32_t i = 0; i < rhs.size_; ++i) {
        if (rhs.get(i)) set(i, !get(i));
    }
    return *this;
}

BitsetD operator<<(const BitsetD &lhs, const std::int32_t shift)
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


BitsetD::BitsetD(BitsetD&& src) noexcept
{
    size_ = src.size_;
    databasa = std::move(src.databasa);
    src.size_ = 0;
}


BitsetD& BitsetD::operator=(BitsetD&& rhs) noexcept
{
    if (this != &rhs) {
        size_ = rhs.size_;
        databasa = std::move(rhs.databasa);
        rhs.size_ = 0;
    }
    return *this;
}
