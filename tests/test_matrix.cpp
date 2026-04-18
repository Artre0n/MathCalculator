#include <QTest>
#include "../Matrix.h"

class TestMatrix : public QObject
{
    Q_OBJECT

private slots:
    void test_constructor_default()
    {
        Matrix<double> m;
        QCOMPARE(m.getRows(), 0u);
        QCOMPARE(m.getCols(), 0u);
    }

    void test_constructor_size()
    {
        Matrix<double> m(3, 4);
        QCOMPARE(m.getRows(), 3u);
        QCOMPARE(m.getCols(), 4u);
    }

    void test_addition()
    {
        Matrix<double> a(2, 2);
        Matrix<double> b(2, 2);

        a(0, 0) = 1; a(0, 1) = 2;
        a(1, 0) = 3; a(1, 1) = 4;

        b(0, 0) = 5; b(0, 1) = 6;
        b(1, 0) = 7; b(1, 1) = 8;

        Matrix<double> result = a + b;

        QCOMPARE(result(0, 0), 6.0);
        QCOMPARE(result(0, 1), 8.0);
        QCOMPARE(result(1, 0), 10.0);
        QCOMPARE(result(1, 1), 12.0);
    }

    void test_subtraction()
    {
        Matrix<double> a(2, 2);
        Matrix<double> b(2, 2);

        a(0, 0) = 5; a(0, 1) = 6;
        a(1, 0) = 7; a(1, 1) = 8;

        b(0, 0) = 1; b(0, 1) = 2;
        b(1, 0) = 3; b(1, 1) = 4;

        Matrix<double> result = a - b;

        QCOMPARE(result(0, 0), 4.0);
        QCOMPARE(result(0, 1), 4.0);
        QCOMPARE(result(1, 0), 4.0);
        QCOMPARE(result(1, 1), 4.0);
    }

    void test_multiplication_matrix()
    {
        Matrix<double> a(2, 3);
        Matrix<double> b(3, 2);

        a(0, 0) = 1; a(0, 1) = 2; a(0, 2) = 3;
        a(1, 0) = 4; a(1, 1) = 5; a(1, 2) = 6;

        b(0, 0) = 7; b(0, 1) = 8;
        b(1, 0) = 9; b(1, 1) = 10;
        b(2, 0) = 11; b(2, 1) = 12;

        Matrix<double> result = a * b;

        QCOMPARE(result(0, 0), 58.0);
        QCOMPARE(result(0, 1), 64.0);
        QCOMPARE(result(1, 0), 139.0);
        QCOMPARE(result(1, 1), 154.0);
    }

    void test_transpose()
    {
        Matrix<double> a(2, 3);
        a(0, 0) = 1; a(0, 1) = 2; a(0, 2) = 3;
        a(1, 0) = 4; a(1, 1) = 5; a(1, 2) = 6;

        Matrix<double> transposed = a.transpose();

        QCOMPARE(transposed.getRows(), 3u);
        QCOMPARE(transposed.getCols(), 2u);
        QCOMPARE(transposed(0, 0), 1.0);
        QCOMPARE(transposed(0, 1), 4.0);
        QCOMPARE(transposed(1, 0), 2.0);
        QCOMPARE(transposed(1, 1), 5.0);
        QCOMPARE(transposed(2, 0), 3.0);
        QCOMPARE(transposed(2, 1), 6.0);
    }

    void test_determinant()
    {
        Matrix<double> m(2, 2);
        m(0, 0) = 1; m(0, 1) = 2;
        m(1, 0) = 3; m(1, 1) = 4;

        QCOMPARE(m.determinant(), -2.0);
    }

    void test_identity()
    {
        Matrix<double> I = Matrix<double>::identity(3);

        QCOMPARE(I.getRows(), 3u);
        QCOMPARE(I.getCols(), 3u);

        for (size_t i = 0; i < 3; ++i) {
            QCOMPARE(I(i, i), 1.0);
        }
    }
};

QTEST_MAIN(TestMatrix)
#include "test_matrix.moc"