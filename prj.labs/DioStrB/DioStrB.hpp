#pragma once
#ifndef DIOSTRB_HPP
#define DIOSTRB_HPP

#include <iostream>
#include <string>

class DioStrB {
    public:
        DioStrB() = default;
        ~DioStrB() = default;
        DioStrB(const DioStrB&) = default;
        void save(const std::string& str);
        std::ostream& writeti(std::ostream& out) const;
        std::istream& readfrom(std::istream& in);
        const std::string& val() const;
        std::string& val();

    private:
        std::string str_string;
};

inline std::ostream& operator<<(std::ostream& ostrm, const DioStrB& rhs) {
    return rhs.writeti(ostrm);
}

inline std::istream& operator>>(std::istream& istrm, DioStrB& rhs) {
    return rhs.readfrom(istrm);
}

#endif
