#include "MainWindow.h"
#include "MatrixTab.h"
#include <QVBoxLayout>
#include <QLabel>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setWindowTitle("Математический Калькулятор");
    resize(900, 700);

    m_tabWidget = new QTabWidget(this);
    setCentralWidget(m_tabWidget);

    // Вкладка 1: Матрицы
    m_matrixTab = new MatrixTab();
    m_tabWidget->addTab(m_matrixTab, "Матрицы");

    // Остальные вкладки
    m_tabWidget->addTab(createPlaceholder("Модуль производных в разработке..."), "Производные");
    m_tabWidget->addTab(createPlaceholder("Модуль интегралов в разработке..."), "Интегралы");
    m_tabWidget->addTab(createPlaceholder("Модуль полиномов в разработке..."), "Полиномы");
}

MainWindow::~MainWindow() {}

QWidget* MainWindow::createPlaceholder(const QString& text) {
    QWidget *w = new QWidget();
    QVBoxLayout *l = new QVBoxLayout(w);
    l->addWidget(new QLabel(text));
    l->setAlignment(Qt::AlignCenter);
    return w;
}