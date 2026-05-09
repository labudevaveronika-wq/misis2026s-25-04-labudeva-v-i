#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest/doctest.h"
#include "../prj.labs/dio/dio.hpp"
#include <sstream>
#include <string>

TEST_CASE("DioStrb: Binary I/O") {
    DioStrb d;
    std::stringstream ss(std::ios::binary | std::ios::in | std::ios::out);

    SUBCASE("Round-trip") {
        std::string original = "Binary\0Test";
        d.save(original);
        ss << d;

        DioStrb d_res;
        ss >> d_res;
        CHECK(d_res.val() == original);
    }

    SUBCASE("Format check") {
        d.save("ABC");
        ss << d;
        CHECK(ss.str().size() == sizeof(size_t) + 3);
    }

    SUBCASE("Empty string") {
        d.save("");
        ss << d;
        
        DioStrb d_res;
        ss >> d_res;
        CHECK(d_res.val() == "");
        CHECK(ss.str().size() == sizeof(size_t));
    }

    SUBCASE("Accessors") {
        d.save("const");
        const DioStrb& cd = d;
        CHECK(cd.val() == "const");
        
        d.val() = "mutable";
        CHECK(d.val() == "mutable");
    }
}

TEST_CASE("DioStrt: Text I/O with Escaping") {
    DioStrt d;
    std::stringstream ss;

    SUBCASE("Escaping") {
        d.save("First\nSecond\\Third");
        ss << d;
        CHECK(ss.str() == "\"First\\nSecond\\\\Third\"");

        DioStrt d_res;
        ss >> d_res;
        CHECK(d_res.val() == "First\nSecond\\Third");
    }

    SUBCASE("Garbage handling") {
        std::istringstream iss("random_prefix \"target_text\" suffix");
        iss >> d;
        CHECK(d.val() == "target_text");
    }

    SUBCASE("Empty quotes") {
        d.save("");
        ss << d;
        CHECK(ss.str() == "\"\"");
        
        DioStrt d_res;
        ss >> d_res;
        CHECK(d_res.val() == "");
    }

    SUBCASE("Unknown sequence") {
        std::istringstream iss("\"unknown\\z sequence\"");
        iss >> d;
        CHECK(d.val() == "unknownz sequence");
    }
}

TEST_CASE("DioStrt: Const access") {
    DioStrt d;
    d.save("read-only");
    const DioStrt& cd = d;
    CHECK(cd.val() == "read-only");
}
