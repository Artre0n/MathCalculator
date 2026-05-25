#pragma once

#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#include "test_utils.h"
#include "../core/Complex.h"
#include <cmath>
#include <iostream>



using C = Complex<double>;

void test_complex_constructors() {
    std::cout << "  [SUITE] Constructors\n";
    C z1; CHECK_EQ(z1.getReal(), 0.0); CHECK_EQ(z1.getImag(), 0.0);
    C z2(3.0, 4.0); CHECK_EQ(z2.getReal(), 3.0); CHECK_EQ(z2.getImag(), 4.0);
    C z3(5.0); CHECK_EQ(z3.getReal(), 5.0); CHECK_EQ(z3.getImag(), 0.0);
    C z4(z2); CHECK(z4.getReal() == z2.getReal() && z4.getImag() == z2.getImag());
}

void test_complex_arithmetic() {
    std::cout << "  [SUITE] Arithmetic\n";
    C a(1, 2), b(3, 4);
    C sum = a + b; CHECK_EQ(sum.getReal(), 4.0); CHECK_EQ(sum.getImag(), 6.0);
    C diff = b - a; CHECK_EQ(diff.getReal(), 2.0); CHECK_EQ(diff.getImag(), 2.0);
    C prod = a * b; CHECK_EQ(prod.getReal(), -5.0); CHECK_EQ(prod.getImag(), 10.0);
    C div = C(10, 5) / C(2, 1); CHECK_EQ(div.getReal(), 5.0); CHECK_EQ(div.getImag(), 0.0);
    CHECK_THROW(C(1, 1) / C(0, 0));
    C scaled = a * 2.0; CHECK_EQ(scaled.getReal(), 2.0); CHECK_EQ(scaled.getImag(), 4.0);
}

void test_complex_advanced() {
    std::cout << "  [SUITE] Advanced\n";
    C z(3, 4);
    CHECK_NEAR(z.abs(), 5.0, 1e-10);
    CHECK_NEAR(z.arg(), std::atan2(4, 3), 1e-10);
    C conj = z.conjugate(); CHECK_EQ(conj.getReal(), 3.0); CHECK_EQ(conj.getImag(), -4.0);
    C sq = C(1, 1).pow(2); CHECK_NEAR(sq.getReal(), 0.0, 1e-10); CHECK_NEAR(sq.getImag(), 2.0, 1e-10);
    C euler = C(0, M_PI).exp(); CHECK_NEAR(euler.getReal(), -1.0, 1e-10); CHECK_NEAR(euler.getImag(), 0.0, 1e-10);
    CHECK(C(1, 2).getReal() == 1.0 && C(1, 2).getImag() == 2.0);
}