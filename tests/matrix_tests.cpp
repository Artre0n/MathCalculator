#include "test_utils.h"
#include "../core/Matrix.h"
#include <cmath>
#include <iostream>

using M = Matrix<double>;

void test_matrix_constructors() {
    std::cout << "  [SUITE] Constructors\n";
    M m1(2, 3);
    CHECK_EQ(m1.getRows(), 2);
    CHECK_EQ(m1.getCols(), 3);

    M m2(2, 2);
    m2(0, 0) = 1.0; m2(0, 1) = 2.0;
    m2(1, 0) = 3.0; m2(1, 1) = 4.0;
    CHECK_EQ(m2(0, 0), 1.0); CHECK_EQ(m2(1, 1), 4.0);
    CHECK_THROW(m2(10, 0));
}

void test_matrix_arithmetic() {
    std::cout << "  [SUITE] Arithmetic\n";
    M a(2, 2);
    a(0, 0) = 1.0; a(0, 1) = 2.0;
    a(1, 0) = 3.0; a(1, 1) = 4.0;

    M b(2, 2);
    b(0, 0) = 5.0; b(0, 1) = 6.0;
    b(1, 0) = 7.0; b(1, 1) = 8.0;

    M sum = a + b;
    CHECK_EQ(sum(0, 0), 6.0); CHECK_EQ(sum(1, 1), 12.0);

    M diff = b - a;
    CHECK_EQ(diff(0, 0), 4.0); CHECK_EQ(diff(1, 1), 4.0);

    M prod = a * b;
    CHECK_EQ(prod(0, 0), 19.0); CHECK_EQ(prod(0, 1), 22.0);
    CHECK_EQ(prod(1, 0), 43.0); CHECK_EQ(prod(1, 1), 50.0);

    M scaled = a * 2.0;
    CHECK_EQ(scaled(0, 0), 2.0); CHECK_EQ(scaled(1, 1), 8.0);
}

void test_matrix_advanced() {
    std::cout << "  [SUITE] Advanced\n";
    M m(2, 3);
    m(0, 0) = 1.0; m(0, 1) = 2.0; m(0, 2) = 3.0;
    m(1, 0) = 4.0; m(1, 1) = 5.0; m(1, 2) = 6.0;

    M t = m.transpose();
    CHECK_EQ(t.getRows(), 3); CHECK_EQ(t.getCols(), 2);
    CHECK_EQ(t(0, 1), 4.0); CHECK_EQ(t(2, 0), 3.0);

    M m2(2, 2);
    m2(0, 0) = 2.0; m2(0, 1) = 3.0;
    m2(1, 0) = 1.0; m2(1, 1) = 4.0;
    CHECK_EQ(m2.determinant(), 5.0);

    M inv_src(2, 2);
    inv_src(0, 0) = 4.0; inv_src(0, 1) = 7.0;
    inv_src(1, 0) = 2.0; inv_src(1, 1) = 6.0;
    M inv = inv_src.inverse();
    M identity = inv_src * inv;
    CHECK_NEAR(identity(0, 0), 1.0, 1e-10);
    CHECK_NEAR(identity(1, 1), 1.0, 1e-10);
    CHECK_NEAR(identity(0, 1), 0.0, 1e-10);

    M I = M::identity(3);
    for (size_t i = 0; i < 3; ++i)
        for (size_t j = 0; j < 3; ++j)
            CHECK_EQ(I(i, j), (i == j) ? 1.0 : 0.0);

    M diag(2, 2);
    diag(0, 0) = 2.0; diag(0, 1) = 0.0;
    diag(1, 0) = 0.0; diag(1, 1) = 2.0;
    M sq = diag.power(2);
    CHECK_EQ(sq(0, 0), 4.0); CHECK_EQ(sq(1, 1), 4.0);

    M norm_m(2, 2);
    norm_m(0, 0) = -3.0; norm_m(0, 1) = 4.0;
    norm_m(1, 0) = 1.0; norm_m(1, 1) = -2.0;
    CHECK_EQ(norm_m.norm(), 7.0);

    M x(2, 2); x(0, 0) = 1.0; x(0, 1) = 2.0; x(1, 0) = 3.0; x(1, 1) = 4.0;
    M y(2, 2); y(0, 0) = 1.0; y(0, 1) = 2.0; y(1, 0) = 3.0; y(1, 1) = 4.0;
    M z(2, 2); z(0, 0) = 1.0; z(0, 1) = 2.0; z(1, 0) = 3.0; z(1, 1) = 5.0;
    CHECK(x == y); CHECK(x != z);
}