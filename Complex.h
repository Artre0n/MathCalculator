#pragma once

#include <iostream>
#include <cmath>

template<typename T>
class Complex {
private:
    T real;
    T imag;

public:
    Complex() {
        real = T(0);
        imag = T(0);
    }

    Complex(const T& r) {
        real = r;
        imag = T(0);
    }

    Complex(const T& r, const T& i) {
        real = r;
        imag = i;
    }

    Complex(const Complex& other) {
        real = other.real;
        imag = other.imag;
    }


    Complex& operator=(const Complex& other) {
        if (this != &other) {
            real = other.real;
            imag = other.imag;
        }
        return *this;
    }


    T getReal() const { return real; }
    T getImag() const { return imag; }
    void setReal(const T& r) { real = r; }
    void setImag(const T& i) { imag = i; }


    Complex operator+(const Complex& other) const {
        return Complex(real + other.real, imag + other.imag);
    }

    Complex operator-(const Complex& other) const {
        return Complex(real - other.real, imag - other.imag);
    }

    Complex operator*(const Complex& other) const {
        return Complex(real * other.real - imag * other.imag, real * other.imag + imag * other.real);
    }

    Complex operator/(const Complex& other) const {
        T denominator = other.real * other.real + other.imag * other.imag;

        if(denominator == T(0)) {
            throw std::invalid_argument("ÐÐµÐ»ÐµÐ½Ð¸Ðµ Ð½Ð° Ð½ÑÐ»ÐµÐ²Ð¾Ðµ ÐºÐ¾Ð¼Ð¿Ð»ÐµÐºÑÐ½Ð¾Ðµ ÑÐ¸ÑÐ»Ð¾");
        }

        return Complex((real * other.real + imag * other.imag) / denominator, (imag * other.real - real * other.imag) / denominator);
    }

    Complex operator+(const T& scalar) const {
        return Complex(real + scalar, imag);
    }

    Complex operator-(const T& scalar) const {
        return Complex(real - scalar, imag);
    }

    Complex operator*(const T& scalar) const {
        return Complex(real * scalar, imag * scalar);
    }

    Complex operator/(const T& scalar) const {
        if(scalar == T(0)) {
            throw std::invalid_argument("ÐÐµÐ»ÐµÐ½Ð¸Ðµ Ð½Ð° Ð½Ð¾Ð»Ñ");
        }
        return Complex(real / scalar, imag / scalar);
    }


    friend Complex operator+(const T& scalar, const Complex& z) {
        return Complex(scalar + z.real, z.imag);
    }

    friend Complex operator-(const T& scalar, const Complex& z) {
        return Complex(scalar - z.real, -z.imag);
    }

    friend Complex operator*(const T& scalar, const Complex& z) {
        return Complex(scalar * z.real, scalar * z.imag);
    }

    friend Complex operator/(const T& scalar, const Complex& z) {
        T denominator = z.real * z.real + z.imag * z.imag;
        if (denominator == T(0)) throw std::runtime_error("ÐÐµÐ»ÐµÐ½Ð¸Ðµ Ð½Ð° Ð½Ð¾Ð»Ñ");
        return Complex(
            scalar * z.real / denominator,
            -scalar * z.imag / denominator
            );
    }

    friend std::ostream& operator<<(std::ostream& os, const Complex& z) {
        os << z.toString();
        return os;
    }

    Complex operator+=(const Complex& other) {
        real += other.real;
        imag += other.imag;
        return *this;
    }

    Complex operator-=(const Complex& other) {
        real -= other.real;
        imag -= other.imag;
        return *this;
    }

    Complex operator*=(const Complex& other) {
        T r = real * other.real - imag * other.imag;
        T i = real * other.imag + imag * other.real;
        real = r;
        imag = i;
        return *this;
    }

    Complex operator/=(const Complex& other) {
        T denominator = other.real * other.real + other.imag * other.imag;
        if(denominator == T(0)) {
            throw std::invalid_argument("ÐÐµÐ»ÐµÐ½Ð¸Ðµ Ð½Ð° Ð½ÑÐ»ÐµÐ²Ð¾Ðµ ÐºÐ¾Ð¼Ð¿Ð»ÐµÐºÑÐ½Ð¾Ðµ ÑÐ¸ÑÐ»Ð¾");
        }
        T r = (real * other.real + imag * other.imag) / denominator;
        T i = (imag * other.real - real * other.imag) / denominator;
        real = r;
        imag = i;
        return *this;
    }

    Complex operator+=(const T& scalar) {
        real += scalar;
        return *this;
    }

    Complex operator-=(const T& scalar) {
        real -= scalar;
        return *this;
    }

    Complex operator*=(const T& scalar) {
        real *= scalar;
        imag *= scalar;
        return *this;
    }

    Complex operator/=(const T& scalar) {
        if(scalar == T(0)) {
            throw std::invalid_argument("ÐÐµÐ»ÐµÐ½Ð¸Ðµ Ð½Ð° Ð½Ð¾Ð»Ñ");
        }
        real /= scalar;
        imag /= scalar;
        return *this;
    }

    Complex operator-() const {
        return Complex(-real, -imag);
    }

    //ÐÐ¾Ð´ÑÐ»Ñ ÐºÐ¾Ð¼Ð¿Ð»ÐµÐºÑÐ½Ð¾Ð³Ð¾ ÑÐ¸ÑÐ»Ð°
    T abs() const {
        return std::sqrt(real * real + imag * imag);
    }

    //ÐÑÐ³ÑÐ¼ÐµÐ½Ñ ÐºÐ¾Ð¼Ð¿Ð»ÐµÐºÑÐ½Ð¾Ð³Ð¾ ÑÐ¸ÑÐ»Ð°
    T arg() const {
        return std::atan2(imag, real);
    }

    //ÐÐ¾Ð¼Ð¿Ð»ÐµÐºÑÐ½Ð¾Ðµ ÑÐ¾Ð¿ÑÑÐ¶ÐµÐ½Ð¸Ðµ
    Complex conjugate() const {
        return Complex(real, -imag);
    }

    // ÐÐ²Ð°Ð´ÑÐ°Ñ Ð¼Ð¾Ð´ÑÐ»Ñ
    T norm() const {
        return real * real + imag * imag;
    }


    // ÐÐ¿ÐµÑÐ°ÑÐ¾ÑÑ ÑÑÐ°Ð²Ð½ÐµÐ½Ð¸Ñ
    bool operator==(const Complex& other) const {
        return real == other.real && imag == other.imag;
    }

    bool operator!=(const Complex& other) const {
        return !(*this == other);
    }

    bool operator<(const Complex& other) const {
        return abs() < other.abs();
    }

    bool operator>(const Complex& other) const {
        return abs() > other.abs();
    }

    Complex pow(int n) const {
        if (n == 0) {
            return Complex(T(1), T(0));
        }

        if (n < 0) {
            return Complex(T(1), T(0)) / pow(-n);
        }

        Complex result = *this;
        for (int i = 1; i < n; ++i) {
            result = result * *this;
        }
        return result;
    }

    // ÐÐ¾ÑÐµÐ½Ñ n-Ð¹ ÑÑÐµÐ¿ÐµÐ½Ð¸ (Ð²Ð¾Ð·Ð²ÑÐ°ÑÐ°ÐµÑ Ð³Ð»Ð°Ð²Ð½ÑÐ¹ ÐºÐ¾ÑÐµÐ½Ñ)
    Complex root(int n) const {
        if (n <= 0) {
            throw std::invalid_argument("Ð¡ÑÐµÐ¿ÐµÐ½Ñ ÐºÐ¾ÑÐ½Ñ Ð´Ð¾Ð»Ð¶Ð½Ð° Ð±ÑÑÑ Ð¿Ð¾Ð»Ð¾Ð¶Ð¸ÑÐµÐ»ÑÐ½Ð¾Ð¹");
        }

        T r = std::pow(abs(), T(1) / n);
        T theta = arg() / n;

        return Complex(r * std::cos(theta), r * std::sin(theta));
    }

    // Ð­ÐºÑÐ¿Ð¾Ð½ÐµÐ½ÑÐ° ÐºÐ¾Ð¼Ð¿Ð»ÐµÐºÑÐ½Ð¾Ð³Ð¾ ÑÐ¸ÑÐ»Ð°
    Complex exp() const {
        T e_real = std::exp(real);
        return Complex(e_real * std::cos(imag), e_real * std::sin(imag));
    }

    // ÐÐ°ÑÑÑÐ°Ð»ÑÐ½ÑÐ¹ Ð»Ð¾Ð³Ð°ÑÐ¸ÑÐ¼
    Complex log() const {
        if (real == T(0) && imag == T(0)) {
            throw std::runtime_error("ÐÐ¾Ð³Ð°ÑÐ¸ÑÐ¼ Ð½ÑÐ»Ñ Ð½Ðµ Ð¾Ð¿ÑÐµÐ´ÐµÐ»ÑÐ½");
        }
        return Complex(std::log(abs()), arg());
    }


    // Ð¢ÑÐ¸Ð³Ð¾Ð½Ð¾Ð¼ÐµÑÑÐ¸ÑÐµÑÐºÐ¸Ðµ ÑÑÐ½ÐºÑÐ¸Ð¸
    Complex sin() const {
        return Complex(
            std::sin(real) * std::cosh(imag),
            std::cos(real) * std::sinh(imag)
            );
    }

    Complex cos() const {
        return Complex(
            std::cos(real) * std::cosh(imag),
            -std::sin(real) * std::sinh(imag)
            );
    }

    Complex tan() const {
        return sin() / cos();
    }

    // ÐÐ¸Ð¿ÐµÑÐ±Ð¾Ð»Ð¸ÑÐµÑÐºÐ¸Ðµ ÑÑÐ½ÐºÑÐ¸Ð¸
    Complex sinh() const {
        return Complex(
            std::sinh(real) * std::cos(imag),
            std::cosh(real) * std::sin(imag)
            );
    }

    Complex cosh() const {
        return Complex(
            std::cosh(real) * std::cos(imag),
            std::sinh(real) * std::sin(imag)
            );
    }

    Complex tanh() const {
        return sinh() / cosh();
    }

    // ÐÐ²Ð°Ð´ÑÐ°ÑÐ½ÑÐ¹ ÐºÐ¾ÑÐµÐ½Ñ
    Complex sqrt() const {
        return root(2);
    }

    // Ð¡ÑÑÐ¾ÐºÐ¾Ð²Ð¾Ðµ Ð¿ÑÐµÐ´ÑÑÐ°Ð²Ð»ÐµÐ½Ð¸Ðµ
    std::string toString() const {
        std::stringstream ss;

        if (imag == T(0)) {
            ss << real;
        }
        else if (real == T(0)) {
            ss << imag << "i";
        }
        else {
            ss << real;
            if (imag > T(0)) {
                ss << "+" << imag << "i";
            }
            else {
                ss << imag << "i";
            }
        }

        return ss.str();
    }

    // ÐÑÐ²Ð¾Ð´ ÐºÐ¾Ð¼Ð¿Ð»ÐµÐºÑÐ½Ð¾Ð³Ð¾ ÑÐ¸ÑÐ»Ð°
    void print() const {
        std::cout << toString();
    }

    // Ð¡ÑÐ°ÑÐ¸ÑÐµÑÐºÐ¸Ðµ Ð¼ÐµÑÐ¾Ð´Ñ
    static Complex zero() {
        return Complex(T(0), T(0));
    }

    static Complex one() {
        return Complex(T(1), T(0));
    }

    static Complex i() {
        return Complex(T(0), T(1));
    }
};
