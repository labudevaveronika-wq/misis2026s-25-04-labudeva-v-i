#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest/doctest.h"
#include "../prj.labs/stackl/stackl.hpp"
#include <iostream>



TEST_CASE("StackL: Constructor and Basic State") {
    StackL s;
    CHECK(s.is_empty() == true);
}

TEST_CASE("StackL: Push and Top") {
    StackL s;
    s.push(1);
    CHECK(s.is_empty() == false);
    CHECK(s.top() == 1);
    
    s.push(2);
    CHECK(s.top() == 2);

    SUBCASE("Const correctness") {
        const StackL& cs = s;
        CHECK(cs.top() == 2); // Вызов константной версии top()
    }
}

TEST_CASE("StackL: Pop and Clear") {
    StackL s;
    s.push(10);
    s.push(20);
    
    s.pop();
    CHECK(s.top() == 10);
    
    s.pop();
    CHECK(s.is_empty() == true);

    SUBCASE("Clear empty stack") {
        s.clear();
        CHECK(s.is_empty() == true);
    }

    SUBCASE("Clear non-empty stack") {
        s.push(1);
        s.push(2);
        s.clear();
        CHECK(s.is_empty() == true);
    }
}

TEST_CASE("StackL: Exceptions") {
    StackL s;
    CHECK_THROWS_AS(s.top(), std::logic_error);
    
    const StackL cs;
    CHECK_THROWS_AS(cs.top(), std::logic_error);
}

TEST_CASE("StackL: Copy Constructor (Deep Copy)") {
    StackL s1;
    s1.push(1);
    s1.push(2);

    StackL s2(s1);
    CHECK(s2.top() == 2);
    
    s1.push(3);
    CHECK(s1.top() == 3);
    CHECK(s2.top() == 2); // Данные s2 не изменились
}

TEST_CASE("StackL: Assignment Operator Logic") {
    StackL s1;
    s1.push(1);
    s1.push(2);

    StackL s2;
    s2.push(10);

    SUBCASE("Assigning larger to smaller") {
        s2 = s1; 
        CHECK(s2.top() == 2);
        s2.pop();
        CHECK(s2.top() == 1);
    }

    SUBCASE("Assigning smaller to larger") {
        StackL s3;
        s3.push(100);
        s3.push(200);
        s3.push(300);
        
        s3 = s1; // У s3 было 3 узла, станет 2 (проверка логики удаления лишних узлов)
        CHECK(s3.top() == 2);
        s3.pop();
        CHECK(s3.top() == 1);
        CHECK(s3.is_empty() == false);
        s3.pop();
        CHECK(s3.is_empty() == true);
    }

    SUBCASE("Self-assignment") {
        s1 = s1;
        CHECK(s1.top() == 2);
    }

    SUBCASE("Assigning empty stack") {
        StackL empty_stack;
        s1 = empty_stack;
        CHECK(s1.is_empty() == true);
    }
}
