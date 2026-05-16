#pragma once

#include <QWidget>
#include <QTableWidget>
#include <QSpinBox>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTextEdit>
#include <QHeaderView>
#include <QMessageBox>
#include <QFrame>
#include "OperationHistory.h"

using MatrixD = Matrix<double>;

class MatrixTab : public QWidget {
    Q_OBJECT
public:
    explicit MatrixTab(QWidget* parent = nullptr);

private slots:
    void onCalculate();          // Вычислить результат выбранной операции
    void onClearAll();           // Очистить все поля
    void onClearA();             // Очистить только матрицу A
    void onClearB();             // Очистить только матрицу B
    void onResizeMatrixA();      // Изменён размер матрицы A
    void onResizeMatrixB();      // Изменён размер матрицы B
    void onOperationChanged(int index);  // Изменена операция
    void showHistory();          // Показать окно истории операций
    void onRandomFillA();       // Заполнить A случайными числами
    void onRandomFillB();       // Заполнить B случайными числами

private:
   
    /// Считать матрицу из таблицы
    MatrixD getMatrixFromTable(QTableWidget* table) const;

    /// Записать матрицу в таблицу
    void setMatrixToTable(const MatrixD& mat, QTableWidget* table);

    /// Показать временное цветное уведомление в статус-баре
    void flashStatus(const QString& text, const QString& color);

    /// Проверить, нужна ли матрица B для данной операции
    bool needsMatrixB(int opIndex) const;

    /// Проверить, нужна ли квадратная матрица A
    bool needsSquareA(int opIndex) const;

    /// Элементы управления

    QComboBox* m_operationCombo; // Выпадающий список операций
    QPushButton* m_calcBtn;      // Кнопка "Вычислить"
    QPushButton* m_clearBtn;     // Кнопка "Очистить всё"
    QPushButton* m_historyBtn;   // Кнопка "История"

    QLabel* m_statusLabel;       // Статусная строка внизу
    QTableWidget* m_outputTable;  // Таблица результата

    // Матрица A
    QGroupBox* m_groupA;         // Группа "Матрица A"
    QTableWidget* m_inputTableA; // Таблица ввода A
    QSpinBox* m_rowsSpinA;       // Спинбокс строк A
    QSpinBox* m_colsSpinA;       // Спинбокс столбцов A
    QPushButton* m_randomABtn;   // Случайное заполнение A
    QPushButton* m_clearABtn;  // Очистка A

    // Матрица B
    QGroupBox* m_groupB;         // Группа "Матрица B"
    QTableWidget* m_inputTableB; // Таблица ввода B
    QSpinBox* m_rowsSpinB;       // Спинбокс строк B
    QSpinBox* m_colsSpinB;       // Спинбокс столбцов B
    QPushButton* m_randomBBtn;   // Случайное заполнение B
    QPushButton* m_clearBBtn;  // Очистка B

    // История
    OperationHistoryDialog* m_historyDialog;  // Диалог истории
};