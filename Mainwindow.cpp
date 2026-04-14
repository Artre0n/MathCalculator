#include "MainWindow.h"
#include "MatrixTab.h"
#include "PolynomialTab.h"
#include <QVBoxLayout>
#include <QLabel>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    setWindowTitle("Математический Калькулятор");
    resize(1000, 750);

    m_tabWidget = new QTabWidget(this);
    setCentralWidget(m_tabWidget);

    // Вкладка 1: Матрицы
    m_matrixTab = new MatrixTab();
    m_tabWidget->addTab(m_matrixTab, "Матрицы");

    // Вкладка 2: Полиномы
    m_polynomialTab = new PolynomialTab();
    m_tabWidget->addTab(m_polynomialTab, "Полиномы");

    // Остальные вкладки
    m_tabWidget->addTab(createPlaceholder("Модуль производных в разработке..."), "Производные");
    m_tabWidget->addTab(createPlaceholder("Модуль интегралов в разработке..."), "Интегралы");
}

MainWindow::~MainWindow() {}

QWidget* MainWindow::createPlaceholder(const QString& text) {
    QWidget* w = new QWidget();
    QVBoxLayout* l = new QVBoxLayout(w);
    QLabel* label = new QLabel(text);
    label->setAlignment(Qt::AlignCenter);
    label->setStyleSheet("font-size: 14px; color: gray; padding: 20px;");
    l->addWidget(label);
    return w;
}