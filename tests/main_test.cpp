#include <iostream>
#include "test_utils.h"

// Объявления тестов
void test_complex_constructors();
void test_complex_arithmetic();
void test_complex_advanced();

void test_polynom_constructors();
void test_polynom_arithmetic();
void test_polynom_calculus();

void test_matrix_constructors();
void test_matrix_arithmetic();
void test_matrix_advanced();

int main() {
    std::cout << "=== MathCalculator Tests ===\n\n";

    std::cout << "--- Complex ---\n";
    test_complex_constructors();
    test_complex_arithmetic();
    test_complex_advanced();

    std::cout << "\n--- Polynomial ---\n";
    test_polynom_constructors();
    test_polynom_arithmetic();
    test_polynom_calculus();

    std::cout << "\n--- Matrix ---\n";
    test_matrix_constructors();
    test_matrix_arithmetic();
    test_matrix_advanced();

    std::cout << "\n=== Results ===\n";
    std::cout << "Passed: " << passed() << "\n";
    std::cout << "Failed: " << failed() << "\n";

    return (failed() == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}