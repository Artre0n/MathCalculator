#pragma once

#include <iostream>
#include <vector>
#include <sstream>


template<typename T>
class Polynomial {
private:
    std::vector<T> coefficients;

    void trim() {
        while (coefficients.size() > 1 && coefficients.back() == T(0)) {
            coefficients.pop_back();
        }
        if (coefficients.empty()) {
            coefficients.push_back(T(0));
        }
    }

    bool isZero() const {
        return coefficients.size() == 1 && coefficients[0] == T(0);
    }

public:
    Polynomial() : coefficients(1, T(0)) {}

    Polynomial(const std::vector<T>& coeffs) : coefficients(coeffs) {
        trim();
    }

    Polynomial(const T& constant) : coefficients(1, constant) {
        trim();
    }

    Polynomial(std::initializer_list<T> list) : coefficients(list) {
        trim();
    }

    // Конструктор копирования
    Polynomial(const Polynomial& other) : coefficients(other.coefficients) {}

    Polynomial& operator=(const Polynomial& other) {
        if (this != &other) {
            coefficients = other.coefficients;
        }
        return *this;
    }

    T& operator[](int index) {
        if (index >= coefficients.size()) {
            coefficients.resize(index + 1, T(0));
        }
        return coefficients[index];
    }

    const T& operator[](int index) const {
        if (index >= coefficients.size()) {
            static const T zero = T(0);
            return zero;
        }
        return coefficients[index];
    }

    // Получение степени полинома
    int degree() const {
        return coefficients.size() - 1;
    }

    // Получение коэффициентов
    const std::vector<T>& getCoefficients() const {
        return coefficients;
    }

    // Вычисление значения полинома в точке
    T evaluate(const T& x) const {
        T result = T(0);
        T power = T(1);

        for (size_t i = 0; i < coefficients.size(); ++i) {
            result += coefficients[i] * power;
            power *= x;
        }

        return result;
    }

    // Арифметические операции
    Polynomial operator+(const Polynomial& other) const {
        int maxSize = std::max(coefficients.size(), other.coefficients.size());
        std::vector<T> result(maxSize, T(0));

        for (int i = 0; i < coefficients.size(); ++i) {
            result[i] += coefficients[i];
        }

        for (int i = 0; i < other.coefficients.size(); ++i) {
            result[i] += other.coefficients[i];
        }

        return Polynomial(result);
    }

    Polynomial operator-(const Polynomial& other) const {
        int maxSize = std::max(coefficients.size(), other.coefficients.size());
        std::vector<T> result(maxSize, T(0));

        for (int i = 0; i < coefficients.size(); ++i) {
            result[i] += coefficients[i];
        }

        for (size_t i = 0; i < other.coefficients.size(); ++i) {
            result[i] -= other.coefficients[i];
        }

        return Polynomial(result);
    }

    Polynomial operator*(const Polynomial& other) const {
        int resultDegree = coefficients.size() + other.coefficients.size() - 1;
        std::vector<T> result(resultDegree, T(0));

        for (size_t i = 0; i < coefficients.size(); ++i) {
            for (size_t j = 0; j < other.coefficients.size(); ++j) {
                result[i + j] += coefficients[i] * other.coefficients[j];
            }
        }

        return Polynomial(result);
    }

    Polynomial operator*(const T& scalar) const {
        std::vector<T> result(coefficients.size());

        for (size_t i = 0; i < coefficients.size(); ++i) {
            result[i] = coefficients[i] * scalar;
        }

        return Polynomial(result);
    }

    Polynomial operator/(const Polynomial& other) const {
        if (other.isZero()) {
            throw std::invalid_argument("Деление на нулевой полином");
        }

        if (degree() < other.degree()) {
            return Polynomial(T(0));
        }

        std::vector<T> result(degree() - other.degree() + 1, T(0));
        std::vector<T> remainder = coefficients;

        for (int i = degree() - other.degree(); i >= 0; --i) {
            if (remainder.size() <= i + other.degree()) continue;

            T coeff = remainder[i + other.degree()] / other.coefficients.back();
            result[i] = coeff;

            for (size_t j = 0; j <= other.degree(); ++j) {
                remainder[i + j] -= coeff * other.coefficients[j];
            }
        }

        return Polynomial(result);
    }

    // Операторы присваивания с арифметикой
    Polynomial& operator+=(const Polynomial& other) {
        *this = *this + other;
        return *this;
    }

    Polynomial& operator-=(const Polynomial& other) {
        *this = *this - other;
        return *this;
    }

    Polynomial& operator*=(const Polynomial& other) {
        *this = *this * other;
        return *this;
    }

    Polynomial& operator*=(const T& scalar) {
        *this = *this * scalar;
        return *this;
    }

    Polynomial& operator/=(const Polynomial& other) {
        *this = *this / other;
        return *this;
    }

    // Операторы сравнения
    bool operator==(const Polynomial& other) const {
        if (coefficients.size() != other.coefficients.size()) {
            return false;
        }

        for (size_t i = 0; i < coefficients.size(); ++i) {
            if (coefficients[i] != other.coefficients[i]) {
                return false;
            }
        }

        return true;
    }

    bool operator!=(const Polynomial& other) const {
        return !(*this == other);
    }

    // Вычисление корней (только для квадратных уравнений)
    std::vector<double> roots() const {
        std::vector<double> result;

        if (degree() == 1) {
            if (coefficients[0] != T(0) && coefficients[1] != T(0)) {
                result.push_back(-double(coefficients[0]) / double(coefficients[1]));
            }
        }
        else if (degree() == 2) {
            T a = coefficients[2];
            T b = coefficients[1];
            T c = coefficients[0];

            if (a == T(0)) {
                return result;
            }

            double discriminant = double(b * b - T(4) * a * c);

            if (discriminant > 0) {
                result.push_back((-double(b) - std::sqrt(discriminant)) / (T(2) * double(a)));
                result.push_back((-double(b) + std::sqrt(discriminant)) / (T(2) * double(a)));
            }
            else if (discriminant == 0) {
                result.push_back(-double(b) / (T(2) * double(a)));
            }
        }

        return result;
    }

    std::string toString() const {
        if (isZero()) {
            return "0";
        }

        std::stringstream ss;

        for (int i = coefficients.size() - 1; i >= 0; --i) {
            if (coefficients[i] == T(0)) {
                continue;
            }

            if (i < coefficients.size() - 1 && coefficients[i] > T(0)) {
                ss << "+";
            }

            if (i == 0) {
                ss << coefficients[i];
            }
            else if (i == 1) {
                if (coefficients[i] == T(1)) {
                    ss << "x";
                }
                else if (coefficients[i] == T(-1)) {
                    ss << "-x";
                }
                else {
                    ss << coefficients[i] << "x";
                }
            }
            else {
                if (coefficients[i] == T(1)) {
                    ss << "x^" << i;
                }
                else if (coefficients[i] == T(-1)) {
                    ss << "-x^" << i;
                }
                else {
                    ss << coefficients[i] << "x^" << i;
                }
            }
        }

        return ss.str();
    }


    // Вывод полинома
    void print() const {
        std::cout << toString() << std::endl;
    }

    // Статические методы
    static Polynomial zero() {
        return Polynomial(T(0));
    }

    static Polynomial one() {
        return Polynomial(T(1));
    }

    static Polynomial x() {
        return Polynomial({ T(0), T(1) });
    }
};

// Внешние операторы
template<typename T>
Polynomial<T> operator*(const T& scalar, const Polynomial<T>& poly) {
    return poly * scalar;
}

template<typename T>
Polynomial<T> operator+(const T& scalar, const Polynomial<T>& poly) {
    return poly + Polynomial<T>(scalar);
}

template<typename T>
Polynomial<T> operator-(const T& scalar, const Polynomial<T>& poly) {
    return Polynomial<T>(scalar) - poly;
}

template<typename T>
std::ostream& operator<<(std::ostream& os, const Polynomial<T>& poly) {
    os << poly.toString();
    return os;
}

