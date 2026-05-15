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
		if (denominator == T(0)) throw std::runtime_error("Деление на ноль");
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

	// Квадрат модуля
	T norm() const {
		return real * real + imag * imag;
	}


	// Операторы сравнения
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

	// Корень n-й степени (возвращает главный корень)
	Complex root(int n) const {
		if (n <= 0) {
			throw std::invalid_argument("Степень корня должна быть положительной");
		}

		T r = std::pow(abs(), T(1) / n);
		T theta = arg() / n;

		return Complex(r * std::cos(theta), r * std::sin(theta));
	}

	// Экспонента комплексного числа
	Complex exp() const {
		T e_real = std::exp(real);
		return Complex(e_real * std::cos(imag), e_real * std::sin(imag));
	}

	// Натуральный логарифм
	Complex log() const {
		if (real == T(0) && imag == T(0)) {
			throw std::runtime_error("Логарифм нуля не определён");
		}
		return Complex(std::log(abs()), arg());
	}


	// Тригонометрические функции
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

	// Гиперболические функции
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

	// Квадратный корень
	Complex sqrt() const {
		return root(2);
	}

	// Строковое представление
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

	// Вывод комплексного числа
	void print() const {
		std::cout << toString();
	}

	// Статические методы
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
