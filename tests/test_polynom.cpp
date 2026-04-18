#include <QTest>
#include "../Polynom.h"

class TestPolynomial : public QObject
{
    Q_OBJECT

private slots:

    // ========== ТЕСТ 1: Конструкторы ==========
    void test_constructor_default()
    {
        Polynomial<double> p;

        QCOMPARE(p.degree(), 0);
        QCOMPARE(p[0], 0.0);
    }

    void test_constructor_constant()
    {
        Polynomial<double> p(5.0);

        QCOMPARE(p.degree(), 0);
        QCOMPARE(p[0], 5.0);
    }

    void test_constructor_vector()
    {
        std::vector<double> coeffs = { 1, 2, 3 };  // 1 + 2x + 3x^2
        Polynomial<double> p(coeffs);

        QCOMPARE(p.degree(), 2);
        QCOMPARE(p[0], 1.0);
        QCOMPARE(p[1], 2.0);
        QCOMPARE(p[2], 3.0);
    }

    void test_constructor_initializer_list()
    {
        Polynomial<double> p = { 1, 2, 3, 0, 0 };  // 1 + 2x + 3x^2 (нули отбрасываются)

        QCOMPARE(p.degree(), 2);
        QCOMPARE(p[0], 1.0);
        QCOMPARE(p[1], 2.0);
        QCOMPARE(p[2], 3.0);
    }

    // ========== ТЕСТ 2: Сложение ==========
    void test_addition()
    {
        Polynomial<double> a = { 1, 2, 3 };   // 1 + 2x + 3x^2
        Polynomial<double> b = { 4, 5, 6 };   // 4 + 5x + 6x^2

        Polynomial<double> result = a + b;  // 5 + 7x + 9x^2

        QCOMPARE(result.degree(), 2);
        QCOMPARE(result[0], 5.0);
        QCOMPARE(result[1], 7.0);
        QCOMPARE(result[2], 9.0);
    }

    void test_addition_different_degrees()
    {
        Polynomial<double> a = { 1, 2, 3 };   // степень 2
        Polynomial<double> b = { 4, 5 };      // степень 1 (4 + 5x)

        Polynomial<double> result = a + b;  // 5 + 7x + 3x^2

        QCOMPARE(result.degree(), 2);
        QCOMPARE(result[0], 5.0);
        QCOMPARE(result[1], 7.0);
        QCOMPARE(result[2], 3.0);
    }

    // ========== ТЕСТ 3: Вычитание ==========
    void test_subtraction()
    {
        Polynomial<double> a = { 5, 7, 9 };
        Polynomial<double> b = { 1, 2, 3 };

        Polynomial<double> result = a - b;  // 4 + 5x + 6x^2

        QCOMPARE(result[0], 4.0);
        QCOMPARE(result[1], 5.0);
        QCOMPARE(result[2], 6.0);
    }

    // ========== ТЕСТ 4: Умножение ==========
    void test_multiplication()
    {
        Polynomial<double> a = { 1, 1 };      // 1 + x
        Polynomial<double> b = { 1, -1 };     // 1 - x

        Polynomial<double> result = a * b;  // (1+x)(1-x) = 1 - x^2

        QCOMPARE(result.degree(), 2);
        QCOMPARE(result[0], 1.0);
        QCOMPARE(result[1], 0.0);
        QCOMPARE(result[2], -1.0);
    }

    void test_multiplication_scalar()
    {
        Polynomial<double> a = { 1, 2, 3 };
        Polynomial<double> result = a * 2.0;  // 2 + 4x + 6x^2

        QCOMPARE(result[0], 2.0);
        QCOMPARE(result[1], 4.0);
        QCOMPARE(result[2], 6.0);
    }

    // ========== ТЕСТ 5: Деление ==========
    void test_division()
    {
        Polynomial<double> a = { 1, 2, 1 };   // 1 + 2x + x^2 = (x+1)^2
        Polynomial<double> b = { 1, 1 };      // 1 + x

        Polynomial<double> result = a / b;  // 1 + x

        QCOMPARE(result.degree(), 1);
        QCOMPARE(result[0], 1.0);
        QCOMPARE(result[1], 1.0);
    }

    // ========== ТЕСТ 6: Вычисление значения (evaluate) ==========
    void test_evaluate()
    {
        Polynomial<double> p = { 1, 2, 3 };  // 1 + 2x + 3x^2

        QCOMPARE(p.evaluate(0), 1.0);    // 1 + 0 + 0 = 1
        QCOMPARE(p.evaluate(1), 6.0);    // 1 + 2 + 3 = 6
        QCOMPARE(p.evaluate(2), 17.0);   // 1 + 4 + 12 = 17
    }

    // ========== ТЕСТ 7: Корни ==========
    void test_roots_linear()
    {
        Polynomial<double> p = { 2, 1 };    // 2 + x
        std::vector<double> roots = p.roots();

        QCOMPARE(roots.size(), 1u);
        QCOMPARE(roots[0], -2.0);
    }

    void test_roots_quadratic()
    {
        Polynomial<double> p = { 1, -3, 2 };  // 1 - 3x + 2x^2 = (2x-1)(x-1)
        std::vector<double> roots = p.roots();

        // Корни: 0.5 и 1
        QCOMPARE(roots.size(), 2u);

        // Сортируем для надёжности сравнения
        std::sort(roots.begin(), roots.end());
        QCOMPARE(roots[0], 0.5);
        QCOMPARE(roots[1], 1.0);
    }

    // ========== ТЕСТ 8: Производная ==========
    void test_derivative()
    {
        Polynomial<double> p = { 1, 2, 3 };   // 1 + 2x + 3x^2
        Polynomial<double> deriv = p.derivative();  // 2 + 6x

        QCOMPARE(deriv.degree(), 1);
        QCOMPARE(deriv[0], 2.0);
        QCOMPARE(deriv[1], 6.0);
    }

    // ========== ТЕСТ 9: Интеграл ==========
    void test_integral()
    {
        Polynomial<double> p = { 2, 6 };     // 2 + 6x
        Polynomial<double> integ = p.integral(1.0);  // ∫(2+6x)dx = 1 + 2x + 3x^2

        QCOMPARE(integ.degree(), 2);
        QCOMPARE(integ[0], 1.0);
        QCOMPARE(integ[1], 2.0);
        QCOMPARE(integ[2], 3.0);
    }

    // ========== ТЕСТ 10: Сравнение ==========
    void test_equality()
    {
        Polynomial<double> a = { 1, 2, 3 };
        Polynomial<double> b = { 1, 2, 3 };
        Polynomial<double> c = { 1, 2, 4 };

        QVERIFY(a == b);
        QVERIFY(a != c);
    }

    // ========== ТЕСТ 11: toString ==========
    void test_toString()
    {
        Polynomial<double> p = { 1, 2, 3 };
        std::string str = p.toString();

        // Ожидается "3x^2+2x+1" или подобное (зависит от реализации)
        QVERIFY(!str.empty());

        // Можно проверить наличие ключевых элементов
        QVERIFY(str.find("x^2") != std::string::npos);
        QVERIFY(str.find("x") != std::string::npos);
    }

    // ========== ТЕСТ 12: Статические методы ==========
    void test_static_zero()
    {
        Polynomial<double> p = Polynomial<double>::zero();

        QCOMPARE(p.degree(), 0);
        QCOMPARE(p[0], 0.0);
    }

    void test_static_one()
    {
        Polynomial<double> p = Polynomial<double>::one();

        QCOMPARE(p.degree(), 0);
        QCOMPARE(p[0], 1.0);
    }

    void test_static_x()
    {
        Polynomial<double> p = Polynomial<double>::x();

        QCOMPARE(p.degree(), 1);
        QCOMPARE(p[0], 0.0);
        QCOMPARE(p[1], 1.0);
    }
};

QTEST_MAIN(TestPolynomial)
#include "test_polynom.moc"