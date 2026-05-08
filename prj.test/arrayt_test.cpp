#include <arrayt/arrayT.hpp>
#include <iostream>
#include <sstream>

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>
#include <limits>



TEST_CASE("ArrayT construction and basic operations") {
    SUBCASE("Default constructor") {
        ArrayT<int> arr;
        CHECK(arr.size() == 0);
    }

    SUBCASE("Constructor with size") {
        ArrayT<int> arr(5);
        CHECK(arr.size() == 5);
        for (int i = 0; i < 5; ++i) {
            CHECK(arr[i] == 0); 
        }
    }

    SUBCASE("Copy constructor") {
        ArrayT<int> arr(3);
        arr[0] = 10; arr[1] = 20; arr[2] = 30;
        
        ArrayT<int> copy = arr;
        CHECK(copy.size() == arr.size());
        CHECK(copy[0] == 10);
        
        copy[0] = 100;
        CHECK(arr[0] == 10); 
    }
}

TEST_CASE("ArrayT modifications") {
    ArrayT<int> arr;

    SUBCASE("Resize") {
        arr.resize(3);
        CHECK(arr.size() == 3);
        arr[2] = 5;
        arr.resize(10);
        CHECK(arr[2] == 5);
        CHECK(arr.size() == 10);
    }

    SUBCASE("Insert") {
        arr.resize(2);
        arr[0] = 1;
        arr[1] = 3;
        arr.insert(1, 2); 
        CHECK(arr.size() == 3);
        CHECK(arr[0] == 1);
        CHECK(arr[1] == 2);
        CHECK(arr[2] == 3);
        
        arr.insert(0, 0); 
        CHECK(arr[0] == 0);
        
        arr.insert(4, 4); 
        CHECK(arr[4] == 4);
    }

    SUBCASE("Remove") {
        arr.resize(3);
        arr[0] = 10; arr[1] = 20; arr[2] = 30;
        arr.remove(1); 
        CHECK(arr.size() == 2);
        CHECK(arr[0] == 10);
        CHECK(arr[1] == 30);
    }
}

TEST_CASE("ArrayT exceptions") {
    ArrayT<int> arr(2);
    
    CHECK_THROWS_AS(arr[2], std::out_of_range);
    CHECK_THROWS_AS(arr[-1], std::out_of_range);
    CHECK_THROWS_AS(ArrayT<int>(-5), std::invalid_argument);
}

TEST_CASE("ArrayT with std::string (complex type)") {
    ArrayT<std::string> arr;
    arr.insert(0, "hello");
    arr.insert(1, "world");
    
    CHECK(arr.size() == 2);
    CHECK(arr[0] == "hello");
    
    arr.remove(0);
    CHECK(arr[0] == "world");
}