#include <iostream>
#include <vector>
#include <stdexcept>
#include <cmath>

template<typename T>
class Matrix {
private:
    std::vector<std::vector<T>> data;
    size_t rows;
    size_t cols;

public:
    // Конструкторы
    Matrix() : rows(0), cols(0) {}

    Matrix(size_t rows, size_t cols) : rows(rows), cols(cols) {
        data.resize(rows, std::vector<T>(cols, T()));
    }

    Matrix(const std::vector<std::vector<T>>& mat) {
        if (mat.empty()) {
            rows = cols = 0;
            return;
        }
        rows = mat.size();
        cols = mat[0].size();

        for (const auto& row : mat) {
            if (row.size() != cols) {
                throw std::invalid_argument("Все строки матрицы должны иметь одинаковую длину");
            }
        }
        data = mat;
    }

    // Конструктор копирования
    Matrix(const Matrix& other) : data(other.data), rows(other.rows), cols(other.cols) {}

    // Оператор присваивания
    Matrix& operator=(const Matrix& other) {
        if (this != &other) {
            data = other.data;
            rows = other.rows;
            cols = other.cols;
        }
        return *this;
    }

    // Доступ к элементам
    T& operator()(size_t i, size_t j) {
        if (i >= rows || j >= cols) {
            throw std::out_of_range("Индекс вне границ матрицы");
        }
        return data[i][j];
    }

    const T& operator()(size_t i, size_t j) const {
        if (i >= rows || j >= cols) {
            throw std::out_of_range("Индекс вне границ матрицы");
        }
        return data[i][j];
    }

    // Геттеры для размеров
    size_t getRows() const { return rows; }
    size_t getCols() const { return cols; }

    // Проверка на квадратную матрицу
    bool isSquare() const { return rows == cols; }

    // Арифметические операции
    Matrix operator+(const Matrix& other) const {
        if (rows != other.rows || cols != other.cols) {
            throw std::invalid_argument("Матрицы должны иметь одинаковый размер для сложения");
        }
        Matrix result(rows, cols);
        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < cols; ++j) {
                result(i, j) = data[i][j] + other(i, j);
            }
        }
        return result;
    }

    Matrix operator-(const Matrix& other) const {
        if (rows != other.rows || cols != other.cols) {
            throw std::invalid_argument("Матрицы должны иметь одинаковый размер для вычитания");
        }
        Matrix result(rows, cols);
        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < cols; ++j) {
                result(i, j) = data[i][j] - other(i, j);
            }
        }
        return result;
    }

    Matrix operator*(const Matrix& other) const {
        if (cols != other.rows) {
            throw std::invalid_argument("Количество столбцов первой матрицы должно равняться количеству строк второй");
        }
        Matrix result(rows, other.cols);
        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < other.cols; ++j) {
                T sum = T();
                for (size_t k = 0; k < cols; ++k) {
                    sum += data[i][k] * other(k, j);
                }
                result(i, j) = sum;
            }
        }
        return result;
    }

    Matrix operator*(const T& scalar) const {
        Matrix result(rows, cols);
        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < cols; ++j) {
                result(i, j) = data[i][j] * scalar;
            }
        }
        return result;
    }

    // Операторы присваивания с арифметикой
    Matrix& operator+=(const Matrix& other) {
        *this = *this + other;
        return *this;
    }

    Matrix& operator-=(const Matrix& other) {
        *this = *this - other;
        return *this;
    }

    Matrix& operator*=(const Matrix& other) {
        *this = *this * other;
        return *this;
    }

    Matrix& operator*=(const T& scalar) {
        *this = *this * scalar;
        return *this;
    }

    // Операторы сравнения
    bool operator==(const Matrix& other) const {
        if (rows != other.rows || cols != other.cols) {
            return false;
        }
        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < cols; ++j) {
                if (data[i][j] != other(i, j)) {
                    return false;
                }
            }
        }
        return true;
    }

    bool operator!=(const Matrix& other) const {
        return !(*this == other);
    }

    // Транспонирование
    Matrix transpose() const {
        Matrix result(cols, rows);
        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < cols; ++j) {
                result(j, i) = data[i][j];
            }
        }
        return result;
    }

    // Определитель
    T determinant() const {
        if (!isSquare()) {
            throw std::invalid_argument("Определитель можно вычислить только для квадратной матрицы");
        }
        if (rows == 1) {
            return data[0][0];
        }
        if (rows == 2) {
            return data[0][0] * data[1][1] - data[0][1] * data[1][0];
        }

        T det = T();
        for (size_t j = 0; j < cols; ++j) {
            Matrix<T> minor = getMinor(0, j);
            T sign = (j % 2 == 0) ? T(1) : T(-1);
            det += sign * data[0][j] * minor.determinant();
        }
        return det;
    }

    // Обратная матрица
    Matrix inverse() const {
        if (!isSquare()) {
            throw std::invalid_argument("Обратную матрицу можно вычислить только для квадратной матрицы");
        }

        T det = determinant();
        if (det == T()) {
            throw std::runtime_error("Матрица вырождена, обратной матрицы не существует");
        }

        if (rows == 1) {
            Matrix result(1, 1);
            result(0, 0) = T(1) / data[0][0];
            return result;
        }

        Matrix result(rows, cols);
        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < cols; ++j) {
                Matrix<T> minor = getMinor(i, j);
                T sign = ((i + j) % 2 == 0) ? T(1) : T(-1);
                result(j, i) = sign * minor.determinant() / det;
            }
        }
        return result;
    }

    // Минор матрицы
    Matrix getMinor(size_t row, size_t col) const {
        if (row >= rows || col >= cols) {
            throw std::out_of_range("Индекс вне границ матрицы");
        }

        Matrix result(rows - 1, cols - 1);
        size_t r = 0;
        for (size_t i = 0; i < rows; ++i) {
            if (i == row) continue;
            size_t c = 0;
            for (size_t j = 0; j < cols; ++j) {
                if (j == col) continue;
                result(r, c) = data[i][j];
                ++c;
            }
            ++r;
        }
        return result;
    }

    // Возведение в степень
    Matrix power(int n) const {
        if (!isSquare()) {
            throw std::invalid_argument("Возведение в степень доступно только для квадратных матриц");
        }

        if (n == 0) {
            Matrix result(rows, cols);
            for (size_t i = 0; i < rows; ++i) {
                result(i, i) = T(1);
            }
            return result;
        }

        if (n < 0) {
            return inverse().power(-n);
        }

        Matrix result = *this;
        for (int i = 1; i < n; ++i) {
            result = result * *this;
        }
        return result;
    }

    // Единичная матрица
    static Matrix identity(size_t size) {
        Matrix result(size, size);
        for (size_t i = 0; i < size; ++i) {
            result(i, i) = T(1);
        }
        return result;
    }

    // Нулевая матрица
    static Matrix zeros(size_t rows, size_t cols) {
        return Matrix(rows, cols);
    }

    // Заполнение
    void fill(const T& value) {
        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < cols; ++j) {
                data[i][j] = value;
            }
        }
    }

    // Получение строки
    std::vector<T> getRow(size_t i) const {
        if (i >= rows) {
            throw std::out_of_range("Индекс строки вне границ");
        }
        return data[i];
    }

    // Получение столбца
    std::vector<T> getCol(size_t j) const {
        if (j >= cols) {
            throw std::out_of_range("Индекс столбца вне границ");
        }
        std::vector<T> column(rows);
        for (size_t i = 0; i < rows; ++i) {
            column[i] = data[i][j];
        }
        return column;
    }

    // Норма матрицы
    double norm() const {
        double maxRowSum = 0.0;
        for (size_t i = 0; i < rows; ++i) {
            double rowSum = 0.0;
            for (size_t j = 0; j < cols; ++j) {
                rowSum += std::abs(data[i][j]);
            }
            if (rowSum > maxRowSum) {
                maxRowSum = rowSum;
            }
        }
        return maxRowSum;
    }

    // Вывод матрицы
    void print() const {
        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < cols; ++j) {
                std::cout << data[i][j] << "\t";
            }
            std::cout << std::endl;
        }
    }

    void print(const std::string& delimiter = "\t", int precision = -1) const {
        if (precision >= 0) {
            std::cout.precision(precision);
            std::cout << std::fixed;
        }
        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < cols; ++j) {
                std::cout << data[i][j];
                if (j < cols - 1) std::cout << delimiter;
            }
            std::cout << std::endl;
        }
    }
};

// Умножение скаляра на матрицу
template<typename T>
Matrix<T> operator*(const T& scalar, const Matrix<T>& matrix) {
    return matrix * scalar;
}

// Вывод в поток
template<typename T>
std::ostream& operator<<(std::ostream& os, const Matrix<T>& matrix) {
    for (size_t i = 0; i < matrix.getRows(); ++i) {
        for (size_t j = 0; j < matrix.getCols(); ++j) {
            os << matrix(i, j);
            if (j < matrix.getCols() - 1) os << "\t";
        }
        if (i < matrix.getRows() - 1) os << "\n";
    }
    return os;
}