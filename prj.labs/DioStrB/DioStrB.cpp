#include "DioStrB.hpp"

void DioStrB::save(const std::string& str) {
    str_string = str;
}

std::ostream& DioStrB::writeti(std::ostream& out) const {
    size_t len = str_string.length();
    out.write((char*)&len, sizeof(len));
    out.write(str_string.c_str(), len);
    return out; 
}

std::istream& DioStrB::readfrom(std::istream& in) {
    size_t len = 0;
    if (in.read((char*)&len, sizeof(len))) {
        str_string.resize(len);
        in.read(&str_string[0], len);
    }
    return in; 
}

const std::string& DioStrB::val() const { return str_string; }
std::string& DioStrB::val() { return str_string; }
