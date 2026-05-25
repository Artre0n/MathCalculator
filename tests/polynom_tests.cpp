// tests/polynom_tests.cpp
#include "test_utils.h"       // 🔹 Теперь все проверки берутся отсюда
#include "../core/Polynom.h"  // Убедись, что путь верный (относительно папки tests/)
#include <cmath>
#include <iostream>
#include <initializer_list>   // Нужен для синтаксиса {1, 2, 3}

// 🔹 УБРАНЫ ВСЕ extern void check_... — они конфликтуют с шаблонами из test_utils.h

using P = Polynomial<double>;

void test_polynom_constructors() {
    std::cout << "  [SUITE] Constructors\n";

    P p1;
    CHECK(p1.isZero());

    P p2({ 1, 2, 3 });  // 1 + 2x + 3x²
    CHECK_EQ(p2.degree(), 2u);      // 🔹 u для size_t
    CHECK_EQ(p2[0], 1.0);
    CHECK_EQ(p2[2], 3.0);
    CHECK_EQ(p2[10], 0.0);
}

void test_polynom_arithmetic() {
    std::cout << "  [SUITE] Arithmetic\n";

    P a({ 1, 2 }), b({ 3, 4 });

    P sum = a + b;
    CHECK_EQ(sum[0], 4.0);
    CHECK_EQ(sum[1], 6.0);

    P diff = b - a;
    CHECK_EQ(diff[0], 2.0);
    CHECK_EQ(diff[1], 2.0);

    P sq = P({ 1,1 }) * P({ 1,1 });  // (1+x)²
    CHECK_EQ(sq[0], 1.0);
    CHECK_EQ(sq[1], 2.0);
    CHECK_EQ(sq[2], 1.0);

    P scaled = a * 3.0;
    CHECK_EQ(scaled[0], 3.0);
    CHECK_EQ(scaled[1], 6.0);
}

void test_polynom_calculus() {
    std::cout << "  [SUITE] Calculus\n";

    P p({ 1, 2, 3 });  // 1 + 2x + 3x²
    CHECK_EQ(p.evaluate(2), 17.0);

    P deriv = p.derivative();
    CHECK_EQ(deriv[0], 2.0);
    CHECK_EQ(deriv[1], 6.0);

    P integ = p.integral();
    CHECK_EQ(integ[0], 0.0);
    CHECK_EQ(integ[1], 1.0);
    CHECK_EQ(integ[2], 1.0);
    CHECK_EQ(integ[3], 1.0);

    // Деление
    P dividend({ 1, 0, 1 });
    P divisor({ 1, 1 });
    P quotient = dividend / divisor;
    P remainder = dividend % divisor;
    P check = divisor * quotient + remainder;

    CHECK(check.degree() == dividend.degree());
    for (size_t i = 0; i <= check.degree(); ++i)
        CHECK_NEAR(check[i], dividend[i], 1e-10);

    // Корни
    P linear({ -2, 1 });
    auto roots = linear.roots();
    CHECK(roots.size() == 1u);  // 🔹 size_t сравнение
    CHECK_NEAR(roots[0], 2.0, 1e-10);
}