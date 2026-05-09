#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest/doctest.h"
#include "../prj.labs/stacka/stacka.hpp"
#include <iostream>

TEST_CASE("Default constructor creates empty stack") {
    stacka s;
    CHECK(s.is_empty() == true);
}

TEST_CASE("Push increases size") {
    stacka s;
    s.push(10);
    CHECK(s.is_empty() == false);
    s.push(20);
}

TEST_CASE("Top returns last pushed element") {
    stacka s;
    s.push(42);
    CHECK(s.top() == 42);
    s.push(100);
    CHECK(s.top() == 100);
}

TEST_CASE("Top reference allows modification") {
    stacka s;
    s.push(5);
    s.top() = 10;
    CHECK(s.top() == 10);
}

TEST_CASE("Pop decreases size") {
    stacka s;
    s.push(1);
    s.push(2);
    s.push(3);

    s.pop();
    CHECK(s.top() == 2);

    s.pop();
    CHECK(s.top() == 1);

    s.pop();
    CHECK(s.is_empty() == true);
}

TEST_CASE("Pop on empty stack does nothing") {
    stacka empty;
    empty.pop();
    CHECK(empty.is_empty() == true);
}

TEST_CASE("Push-pop sequence") {
    stacka s;
    for(int i = 0; i < 100; i++) {
        s.push(i);
        CHECK(s.top() == i);
    }

    for(int i = 99; i >= 0; i--) {
        CHECK(s.top() == i);
        s.pop();
    }

    CHECK(s.is_empty() == true);
}

TEST_CASE("Interleaved push and pop") {
    stacka s;
    s.push(1);
    s.push(2);
    s.pop();
    s.push(3);

    CHECK(s.top() == 3);
    s.pop();
    CHECK(s.top() == 1);
}

TEST_CASE("Copy constructor creates independent copy") {
    stacka original;
    original.push(1);
    original.push(2);
    original.push(3);

    stacka copy(original);

    CHECK(copy.top() == 3);
    original.pop();
    CHECK(original.top() == 2);
    CHECK(copy.top() == 3);

    copy.pop();
    CHECK(copy.top() == 2);
    CHECK(original.top() == 2);
}

TEST_CASE("Assignment operator") {
    stacka a;
    a.push(10);
    a.push(20);

    stacka b;
    b.push(30);

    b = a;

    CHECK(b.top() == 20);
    a.pop();
    CHECK(a.top() == 10);
    CHECK(b.top() == 20);

    b.push(100);
    CHECK(b.top() == 100);
    CHECK(a.top() == 10);
}

TEST_CASE("Self-assignment") {
    stacka s;
    s.push(1);
    s.push(2);

    s = s;

    CHECK(s.top() == 2);
    s.pop();
    CHECK(s.top() == 1);
}

TEST_CASE("Clear empty stack") {
    stacka s;
    s.clear();
    CHECK(s.is_empty() == true);
}

TEST_CASE("Clear non-empty stack") {
    stacka s;
    for(int i = 0; i < 10; i++) {
        s.push(i);
    }

    CHECK(s.is_empty() == false);
    s.clear();
    CHECK(s.is_empty() == true);

    s.push(42);
    CHECK(s.top() == 42);
}

TEST_CASE("Top on empty stack throws exception") {
    stacka s;
    CHECK_THROWS_AS(s.top(), std::runtime_error);
}

TEST_CASE("Push-pop exception safety") {
    stacka s;
    for(int i = 0; i < 1000; i++) {
        s.push(i);
    }

    for(int i = 0; i < 1000; i++) {
        s.pop();
    }

    CHECK(s.is_empty() == true);
}

TEST_CASE("Large stack operations") {
    stacka big;
    const int N = 10000;

    for(int i = 0; i < N; i++) {
        big.push(i);
    }

    CHECK(big.top() == N - 1);

    for(int i = N - 1; i >= 0; i--) {
        CHECK(big.top() == i);
        big.pop();
    }

    CHECK(big.is_empty() == true);
}