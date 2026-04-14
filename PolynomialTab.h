#pragma once
#include <QWidget>
#include <QLineEdit>
#include <QSpinBox>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QTextEdit>
#include <QScrollArea>
#include <QInputDialog>
#include "Polynom.h"

using PolynomialD = Polynomial<double>;

class PolynomialTab : public QWidget {
    Q_OBJECT
public:
    explicit PolynomialTab(QWidget* parent = nullptr);

private slots:
    void onCalculate();
    void onClearAll();
    void onDegreeChanged(int newDegree);
    void onOperationChanged(int index);

private:
    QSpinBox* m_degreeSpin;
    std::vector<QLineEdit*> m_coefficientsA;
    std::vector<QLineEdit*> m_coefficientsB;
    QComboBox* m_operationCombo;
    QPushButton* m_calcBtn;
    QPushButton* m_clearBtn;
    QLabel* m_statusLabel;
    QTextEdit* m_resultEdit;
    QGroupBox* m_groupB;
    QWidget* m_scrollContentA;
    QWidget* m_scrollContentB;

    PolynomialD getPolynomialA() const;
    PolynomialD getPolynomialB() const;
    void rebuildCoefficientInputs();
};