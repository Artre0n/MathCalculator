#pragma once

#include <QWidget>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTextEdit>
#include <QFrame>
#include "Complex.h"

using ComplexD = Complex<double>;

class ComplexTab : public QWidget {
    Q_OBJECT
public:
    explicit ComplexTab(QWidget *parent = nullptr);

private slots:
    void onCalculate();          // Вычислить результат
    void onClearAll();           // Очистить все поля
    void onClearA();             // Очистить только A
    void onClearB();             // Очистить только B
    void onOperationChanged(int index);  // Изменена операция
    void onRandomFillA();        // Заполнить A случайными значениями
    void onRandomFillB();        // Заполнить B случайными значениями

private:
    // Вспомогательные методы

    /// Считать комплексное число из полей ввода
    ComplexD getComplexFromEdits(QLineEdit* realEdit, QLineEdit* imagEdit) const;

    /// Записать комплексное число в поля ввода
    void setComplexToEdits(const ComplexD& z, QLineEdit* realEdit, QLineEdit* imagEdit);

    /// Форматировать комплексное число в строку
    QString complexToString(const ComplexD& z) const;

    /// Показать временное цветное уведомление
    void flashStatus(const QString& text, const QString& color);

    /// Проверить, нужно ли число B для операции
    bool needsB(int opIndex) const;

    // Элементы управления

    QComboBox *m_operationCombo; // Выпадающий список операций
    QPushButton *m_calcBtn;      // Кнопка "Вычислить"
    QPushButton *m_clearBtn;     // Кнопка "Очистить всё"

    QLabel *m_statusLabel;       // Статусная строка
    QTextEdit *m_resultEdit;     // Поле результата

    // Комплексное число A
    QGroupBox* m_groupA;
    QLineEdit *m_realA;          // Действительная часть A
    QLineEdit *m_imagA;          // Мнимая часть A
    QLabel *m_previewA;          // Предпросмотр A
    QPushButton *m_randomABtn;
    QPushButton *m_clearABtn;

    // Комплексное число B
    QGroupBox *m_groupB;
    QLineEdit *m_realB;          // Действительная часть B
    QLineEdit *m_imagB;          // Мнимая часть B
    QLabel *m_previewB;          // Предпросмотр B
    QPushButton *m_randomBBtn;
    QPushButton *m_clearBBtn;
};