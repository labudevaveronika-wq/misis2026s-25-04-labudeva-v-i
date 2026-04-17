#pragma once
#ifndef DIOSTRT_HPP
#define DIOSTRT_HPP

#include <iostream>
#include <string>

class DioStrT {
    private:
        std::string str_string;
    public:
        DioStrT() = default;
        ~DioStrT() = default;
        DioStrT(const DioStrT&) = default;
        void save(const std::string& str);
        std::ostream& writeti(std::ostream& out) const;
        std::istream& readfrom(std::istream& in);
        const std::string& val() const;
};

inline std::ostream& operator<<(std::ostream& ostrm, const DioStrT& rhs) {
    return rhs.writeti(ostrm);
}

inline std::istream& operator>>(std::istream& istrm, DioStrT& rhs) {
    return rhs.readfrom(istrm);
}

#endif
