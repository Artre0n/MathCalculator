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
#include <QInputDialog>
#include <QFrame>
#include "Polynom.h"

using PolynomialD = Polynomial<double>;

class PolynomialTab : public QWidget {
    Q_OBJECT
public:
    explicit PolynomialTab(QWidget* parent = nullptr);

private slots:
    void onCalculate();          // Вычислить результат выбранной операции
    void onClearAll();           // Очистить все поля
    void onClearA();             // Очистить только полином A
    void onClearB();             // Очистить только полином B
    void onDegreeAChanged(int newDegree);  // Изменена степень полинома A
    void onDegreeBChanged(int newDegree);  // Изменена степень полинома B
    void onOperationChanged(int index);    // Изменена выбранная операция
    void updatePreviewA();       // Обновить предпросмотр полинома A
    void updatePreviewB();       // Обновить предпросмотр полинома B
    void onRandomFillA();        // Заполнить A случайными коэффициентами
    void onRandomFillB();        // Заполнить B случайными коэффициентами

private:

    /// Построить таблицу ввода коэффициентов заданной степени
    void buildPolynomialTable(QTableWidget* table, int degree);

    /// Считать полином из таблицы (коэффициенты от младшей к старшей степени)
    PolynomialD getPolynomialFromTable(QTableWidget* table) const;

    /// Записать полином в таблицу
    void setPolynomialToTable(const PolynomialD& poly, QTableWidget* table);

    /// Преобразовать полином в HTML-строку с красивыми степенями
    QString polynomialToHtml(const PolynomialD& poly) const;

    /// Преобразовать полином в обычную строку
    QString polynomialToString(const PolynomialD& poly) const;

    /// Показать временное цветное уведомление в статус-баре
    void flashStatus(const QString& text, const QString& color);

    // Элементы управления 

    QSpinBox* m_degreeSpinA;     // Выбор степени полинома A
    QSpinBox* m_degreeSpinB;     // Выбор степени полинома B

    QComboBox* m_operationCombo; // Выпадающий список операций
    QPushButton* m_calcBtn;      // Кнопка "Вычислить"
    QPushButton* m_clearBtn;     // Кнопка "Очистить всё"

    QLabel* m_statusLabel;       // Статусная строка внизу
    QTextEdit* m_resultEdit;     // Поле вывода результата

    // Полином A 
    QGroupBox* m_groupA;         // Группа "Полином A"
    QTableWidget* m_tableA;      // Таблица коэффициентов A
    QLabel* m_previewA;          // Предпросмотр A в виде формулы
    QPushButton* m_randomABtn;   // Кнопка случайного заполнения A
    QPushButton* m_clearABtn;    // Кнопка очистки A

    // Полином B
    QGroupBox* m_groupB;         // Группа "Полином B"
    QTableWidget* m_tableB;      // Таблица коэффициентов B
    QLabel* m_previewB;          // Предпросмотр B в виде формулы
    QPushButton* m_randomBBtn;   // Кнопка случайного заполнения B
    QPushButton* m_clearBBtn;    // Кнопка очистки B
};