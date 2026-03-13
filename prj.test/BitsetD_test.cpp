
#include "../prj.labs/Bitsetd/BitsetD.hpp"
#include <iostream>
#include <sstream>

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "../prj.labs/prj.thirdparty/doctest/doctest.h"
#include <limits>

TEST_CASE("Constructor tests") {
    SUBCASE("Default constructor") {
        BitsetD bs;
        CHECK(bs.size() == 0);
    }

    SUBCASE("Constructor with size and value") {
        BitsetD bs(10, true);
        CHECK(bs.size() == 10);
        for (int i = 0; i < 10; ++i)
            CHECK(bs.get(i) == true);

        BitsetD bs2(33, false);
        CHECK(bs2.size() == 33);
        for (int i = 0; i < 33; ++i)
            CHECK(bs2.get(i) == false);
    }

    SUBCASE("Constructor with mask and size") {
        BitsetD bs(0b1010ULL, 4);
        CHECK(bs.size() == 4);
        CHECK(bs.get(0) == false);
        CHECK(bs.get(1) == true);
        CHECK(bs.get(2) == false);
        CHECK(bs.get(3) == true);

        BitsetD bs2(0xDEADBEEFULL, 40);
        CHECK(bs2.size() == 40);
        // проверка нескольких бит
        CHECK(bs2.get(0) == ((0xDEADBEEFULL >> 0) & 1));
        CHECK(bs2.get(31) == ((0xDEADBEEFULL >> 31) & 1));
        CHECK(bs2.get(32) == ((0xDEADBEEFULL >> 32) & 1));
        // для битов 33-39 должны быть 0, так как mask 64 бита
        for (int i = 32; i < 40; ++i)
            CHECK(bs2.get(i) == ((i < 32) ? ((0xDEADBEEFULL >> i) & 1) : false));

        BitsetD bs3(0xFFFFFFFFFFFFFFFFULL, 70);
        CHECK(bs3.size() == 70);
        // первые 64 бита должны быть 1
        for (int i = 0; i < 64; ++i)
            CHECK(bs3.get(i) == true);
        // биты 64-69 должны быть 0
        for (int i = 64; i < 70; ++i)
            CHECK(bs3.get(i) == false);
    }

    SUBCASE("Copy constructor") {
        BitsetD orig(20, true);
        orig.set(5, false);
        BitsetD copy(orig);
        CHECK(copy.size() == orig.size());
        for (int i = 0; i < orig.size(); ++i)
            CHECK(copy.get(i) == orig.get(i));
    }

    SUBCASE("Constructor throws on negative size") {
        CHECK_THROWS_AS(BitsetD(-5, true), std::invalid_argument);
        CHECK_THROWS_AS(BitsetD(0xFFFFULL, -10), std::invalid_argument);
    }
}

TEST_CASE("set/get and exceptions") {
    BitsetD bs(16, false);

    SUBCASE("set and get") {
    BitsetD bs(16, false);
    
    bs.set(5, true);
    CHECK(bs.get(5) == true);
    
    bs.set(5, false);
    CHECK(bs.get(5) == false);
    
    bs.set(0, true);
    CHECK(bs.get(0) == true);
    
    bs.set(15, true);
    CHECK(bs.get(15) == true);
    
}

    SUBCASE("out of range") {
        BitsetD bs(16, false); 
        CHECK_THROWS_AS(bs.set(16, true), std::invalid_argument);
        CHECK_THROWS_AS(bs.get(-1), std::invalid_argument);
        CHECK_THROWS_AS(bs.set(-5, false), std::invalid_argument);
        CHECK_THROWS_AS(bs.get(16), std::invalid_argument);
    }
}

TEST_CASE("invert and fill") {
    BitsetD bs(10, false);
    bs.set(2, true);
    bs.set(5, true);
    bs.invert();
    CHECK(bs.get(2) == false);
    CHECK(bs.get(5) == false);
    for (int i = 0; i < 10; ++i) {
        if (i != 2 && i != 5)
            CHECK(bs.get(i) == true);
    }

    bs.fill(false);
    for (int i = 0; i < 10; ++i)
        CHECK(bs.get(i) == false);

    bs.fill(true);
    for (int i = 0; i < 10; ++i)
        CHECK(bs.get(i) == true);
}

TEST_CASE("resize") {
    BitsetD bs(10, true);
    bs.set(5, false);
    bs.resize(5, false);   // уменьшаем
    CHECK(bs.size() == 5);
    for (int i = 0; i < 5; ++i)
        CHECK(bs.get(i) == false); // после resize fill(false) обнулил всё

    bs.resize(20, true);
    CHECK(bs.size() == 20);
    for (int i = 0; i < 20; ++i)
        CHECK(bs.get(i) == true);

    // CHECK_THROWS_AS(bs.resize(-1, false), std::invalid_argument);
}

TEST_CASE("shift (cyclic)") {
    BitsetD bs(8, false);
    bs.set(1, true);
    bs.set(3, true);
    // исходный: 0 1 0 1 0 0 0 0  (индекс 0 слева)
    bs.shift(2);
    // циклический сдвиг влево на 2: биты 1->3, 3->5
    CHECK(bs.get(3) == true);
    CHECK(bs.get(5) == true);
    CHECK(bs.get(1) == false);
    CHECK(bs.get(2) == false);

    bs.shift(-3); // вправо на 3 (или циклический вправо)
    // ожидаем, что биты вернутся? лучше проверить конкретно
    BitsetD bs2(8, false);
    bs2.set(2, true);
    bs2.set(4, true);
    bs2.shift(3);
    CHECK(bs2.get(5) == true);
    CHECK(bs2.get(7) == true);
    bs2.shift(-3);
    CHECK(bs2.get(2) == true);
    CHECK(bs2.get(4) == true);

    // сдвиг на размер
    BitsetD bs3(5, true);
    bs3.set(0, false);
    BitsetD copy(bs3);
    bs3.shift(5);
    CHECK(bs3 == copy);
    bs3.shift(0);
    CHECK(bs3 == copy);

    // сдвиг при size==0
    BitsetD empty;
    empty.shift(10); // не должно упасть
    CHECK(empty.size() == 0);
}

TEST_CASE("non-cyclic shift operators <<= and >>=") {
    BitsetD bs(8, false);
    bs.set(2, true);
    bs.set(5, true);
    // индекс: 0 0 1 0 0 1 0 0

    std::cout << "Initial bits: ";
    for(int i = 0; i < 8; i++) std::cout << bs.get(i);
    std::cout << std::endl;

    bs <<= 2;
    std::cout << "After <<=2: ";
    for(int i = 0; i < 8; i++) std::cout << bs.get(i);
    std::cout << std::endl;
    // должно стать: 0 1 0 0 1 0 0 0
    CHECK(bs.get(0) == false);
    CHECK(bs.get(1) == false);
    CHECK(bs.get(2) == false);
    CHECK(bs.get(3) == true);  // бывший 1
    CHECK(bs.get(4) == false);
    CHECK(bs.get(5) == false);
    CHECK(bs.get(6) == true);  // бывший 4
    CHECK(bs.get(7) == false);

    bs >>= 3;
    // сдвиг вправо на 3: биты 3 и 6 сместятся в 0 и 3
    std::cout << "After >>=3: ";
    for(int i = 0; i < 8; i++) std::cout << bs.get(i);
    std::cout << std::endl;
    CHECK(bs.get(0) == true);  // бывший 3
    CHECK(bs.get(3) == true);  // бывший 6
    for (int i = 1; i <= 7; ++i)
        if (i != 0 && i != 3)
            CHECK(bs.get(i) == false);

    // сдвиг на размер и больше
    BitsetD bs2(5, true);
    bs2 <<= 5;
    for (int i = 0; i < 5; ++i)
        CHECK(bs2.get(i) == false);
    bs2 = BitsetD(5, true);
    bs2 >>= 10;
    for (int i = 0; i < 5; ++i)
        CHECK(bs2.get(i) == false);

    // отрицательный сдвиг
    BitsetD bs3(8, false);
    CHECK_THROWS_AS(bs3 <<= -1, std::invalid_argument);
    CHECK_THROWS_AS(bs3 >>= -2, std::invalid_argument);
}

TEST_CASE("compound assignments &, |, ^") {
    BitsetD a(8, false);
    a.set(0, true);
    a.set(2, true);
    a.set(4, true);

    BitsetD b(8, false);
    b.set(1, true);
    b.set(2, true);
    b.set(3, true);

    SUBCASE("&=") {
        BitsetD c(a);
        c &= b;
        CHECK(c.size() == 8);
        CHECK(c.get(0) == false);
        CHECK(c.get(1) == false);
        CHECK(c.get(2) == true);
        CHECK(c.get(3) == false);
        CHECK(c.get(4) == false);
    }

    SUBCASE("|=") {
        BitsetD c(a);
        c |= b;
        CHECK(c.get(0) == true);
        CHECK(c.get(1) == true);
        CHECK(c.get(2) == true);
        CHECK(c.get(3) == true);
        CHECK(c.get(4) == true);
    }

    SUBCASE("^=") {
        BitsetD c(a);
        c ^= b;
        CHECK(c.get(0) == true);
        CHECK(c.get(1) == true);
        CHECK(c.get(2) == false);
        CHECK(c.get(3) == true);
        CHECK(c.get(4) == true);
    }

    SUBCASE("with different sizes") {
        BitsetD a10(10, false);
        a10.set(0, true);
        a10.set(9, true);
        BitsetD b6(6, false);
        b6.set(2, true);
        b6.set(5, true);

        a10 &= b6;
        CHECK(a10.size() == 10);
        CHECK(a10.get(0) == false);
        CHECK(a10.get(2) == true);
        CHECK(a10.get(5) == true);
        CHECK(a10.get(9) == false); // обнулён, т.к. нет соответствующего бита в b6
        for (int i = 1; i < 10; ++i)
            if (i != 2 && i != 5)
                CHECK(a10.get(i) == false);

        BitsetD a8(8, true);
        BitsetD b4(4, false);
        b4.set(1, true);
        a8 |= b4;
        CHECK(a8.size() == 8);
        CHECK(a8.get(0) == true);
        CHECK(a8.get(1) == true);
        CHECK(a8.get(2) == true);
        CHECK(a8.get(3) == true);
        for (int i = 4; i < 8; ++i)
            CHECK(a8.get(i) == true); // остались исходные 1
    }
}

TEST_CASE("comparison operator ==") {
    BitsetD a(10, false);
    a.set(3, true);
    BitsetD b(10, false);
    b.set(3, true);
    CHECK(a == b);
    b.set(5, true);
    CHECK_FALSE(a == b);
    BitsetD c(8, false);
    CHECK_FALSE(a == c);
}

TEST_CASE("free operators ~, <<, >>, &, |, ^") {
    BitsetD a(6, false);
    a.set(0, true);
    a.set(2, true);
    a.set(5, true);

    SUBCASE("operator~") {
        BitsetD inv = ~a;
        CHECK(inv.size() == 6);
        CHECK(inv.get(0) == false);
        CHECK(inv.get(1) == true);
        CHECK(inv.get(2) == false);
        CHECK(inv.get(3) == true);
        CHECK(inv.get(4) == true);
        CHECK(inv.get(5) == false);
    }

    SUBCASE("operator<< and >>") {
        BitsetD left = a << 2;
        CHECK(left.size() == 6);
        CHECK(left.get(2) == true);
        CHECK(left.get(4) == true);
        CHECK(left.get(5) == false); // бит 5 был 1, но при сдвиге влево на 2 вышел за границы
        CHECK(left.get(0) == false);
        CHECK(left.get(1) == false);

        BitsetD right = a >> 2;
        CHECK(right.size() == 6);
        CHECK(right.get(0) == false);
        CHECK(right.get(1) == false);
        CHECK(right.get(2) == false);
        CHECK(right.get(3) == true); // бит 5 стал 3
        CHECK(right.get(4) == false);
        CHECK(right.get(5) == false);

        // сдвиг на отрицательное
        CHECK_THROWS_AS(a << -1, std::invalid_argument);
        CHECK_THROWS_AS(a >> -3, std::invalid_argument);
    }

    SUBCASE("operator&, |, ^") {
        BitsetD b(6, false);
        b.set(1, true);
        b.set(2, true);
        b.set(4, true);

        BitsetD and_res = a & b;
        CHECK(and_res.size() == 6);
        CHECK(and_res.get(2) == true);
        for (int i = 0; i < 6; ++i)
            if (i != 2) CHECK(and_res.get(i) == false);

        BitsetD or_res = a | b;
        CHECK(or_res.get(0) == true);
        CHECK(or_res.get(1) == true);
        CHECK(or_res.get(2) == true);
        CHECK(or_res.get(3) == false);
        CHECK(or_res.get(4) == true);
        CHECK(or_res.get(5) == true);

        BitsetD xor_res = a ^ b;
        CHECK(xor_res.get(0) == true);
        CHECK(xor_res.get(1) == true);
        CHECK(xor_res.get(2) == false);
        CHECK(xor_res.get(3) == false);
        CHECK(xor_res.get(4) == true);
        CHECK(xor_res.get(5) == true);
    }

    SUBCASE("operators with different sizes") {
        BitsetD a5(5, true);
        BitsetD b8(8, false);
        b8.set(0, true);
        b8.set(7, true);

        BitsetD and_ab = a5 & b8;
        CHECK(and_ab.size() == 8);
        CHECK(and_ab.get(0) == true);
        for (int i = 1; i < 5; ++i)
            CHECK(and_ab.get(i) == false); // a5 true, но b8 false => 0
        for (int i = 5; i < 8; ++i)
            CHECK(and_ab.get(i) == false); // за пределами a5 считаются 0

        BitsetD or_ab = a5 | b8;
        CHECK(or_ab.size() == 8);
        CHECK(or_ab.get(0) == true);
        for (int i = 1; i < 5; ++i)
            CHECK(or_ab.get(i) == true);
        CHECK(or_ab.get(5) == false);
        CHECK(or_ab.get(6) == false);
        CHECK(or_ab.get(7) == true);
    }
}

// TEST_CASE("proxy classes (if operator[] enabled)") {
//     // Этот тест будет работать только если раскомментированы операторы []
//     BitsetD bs(10, false);
//     bs[3] = true;
//     CHECK(bs[3] == true);
//     bs[3] = false;
//     CHECK(bs[3] == false);

//     const BitsetD cbs(bs);
//     CHECK(cbs[3] == false); // const версия
// }

// #include "../prj.labs/Bitsetd/BitsetD.hpp"
// #include <iostream>
// #include <sstream>

// #define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
// #include "../prj.labs/prj.thirdparty/doctest/doctest.h"
// #include <limits>

// // TEST_CASE("Basic BitsetD functionality") {
// //     SUBCASE("Constructors") {
// //         BitsetD empty;
// //         CHECK(empty.size() == 0);

// //         BitsetD T1(16, true);
// //         CHECK(T1.size() == 16);
// //         CHECK(T1.get(0) == 1);
// //         CHECK(T1.get(1) == 1);
// //         CHECK(T1.get(2) == 1);

// //         BitsetD T2(0b1101110100000000);
// //         CHECK(T2.size() == 64);
// //         CHECK(T2.get(0) == 0);
// //         CHECK(T2.get(8) == 1);
// //         CHECK(T2.get(15) == 1);
// //         CHECK(T2.get(63) == 0);

// //         BitsetD T3(T1);
// //         CHECK(T1.size() == 16);
// //         CHECK(T1.get(0) == 1);
// //         CHECK(T1.get(1) == 1);
// //         CHECK(T1.get(2) == 1);
// //     }

// //     SUBCASE("GET SET") {
// //         BitsetD T1(8, false);
// //         CHECK(T1.size() == 8);
// //         CHECK(T1.get(0) == 0);
// //         CHECK(T1.get(1) == 0);
// //         CHECK(T1.get(2) == 0);
// //         T1.set(1, true);
// //         CHECK(T1.get(1) == 1);
// //     }   
// // }
// TEST_CASE("Basic BitsetD Shift functionality"){
//     SUBCASE("от 0 ничего не изменится"){
//         BitsetD bitset(32, true);
//         bitset.set(0, false);
//         BitsetD copy_original(bitset);
//         bitset.shift(0);
//         CHECK(bitset.size()==copy_original.size());
//         for (int i = 0; i < bitset.size(); i++) {
//             CHECK(bitset.get(i) == copy_original.get(i));
//         }
//     }

//     SUBCASE("shift от размера ничего не изменит"){
//         BitsetD bitset(32, true);
//         bitset.set(0, false);
//         std::cout << "До сдвига:\n";
//         std::cout << "bitset.get(0) = " << bitset.get(0) << "\n";
//         std::cout << "bitset.get(31) = " << bitset.get(31) << "\n";
//         BitsetD copy_original(bitset);
//         bitset.shift(32);
//         std::cout << "После сдвига на 32:\n";
//         std::cout << "bitset.get(0) = " << bitset.get(0) << "\n";
//         std::cout << "bitset.get(31) = " << bitset.get(31) << "\n";
//         CHECK(bitset.size()==copy_original.size());
//         for (int i = 0; i < bitset.size(); i++) {
//             CHECK(bitset.get(i) == copy_original.get(i));
//         }
//     }
// }


