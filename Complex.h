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
			throw std::invalid_argument("Деление на нулевое комплексное число");
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
			throw std::invalid_argument("Деление на ноль");
		}
		return Complex(real / scalar, imag / scalar);
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
			throw std::invalid_argument("Деление на нулевое комплексное число");
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
			throw std::invalid_argument("Деление на ноль");
		}
		real /= scalar;
		imag /= scalar;
		return *this;
	}

	Complex operator-() const {
		return Complex(-real, -imag);
	}

	//Модуль комплексного числа
	T abs() const {
		return std::sqrt(real * real + imag * imag);
	}

	//Аргумент комплексного числа
	T arg() const {
		return std::atan2(imag, real);
	}

	//Комплексное сопряжение
	Complex conjugate() const {
		return Complex(real, -imag);
	}
};